/*
    3D Physics Simulations - Blit
    Copyright (C) 2020  Jonathan Bayle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "blit.hpp"
#include "utils.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine::Renderer {

Blitter::Blitter(const Renderer& renderer) :
		renderer(renderer), buf_name(generateArray<glGenBuffers>()), plane()
{}

void Blitter::blit(const Texture& tex, const glm::mat4& model_matrix, const glm::mat3& texture_matrix, const glm::vec4& tint) const
{
	glActiveTexture(GL_TEXTURE0 + Renderer::texture_unit_name);
	glBindTexture(GL_TEXTURE_2D, tex.get_texture_name());
	renderer.set_has_texture(true);

	renderer.set_model_matrix(model_matrix);
	renderer.set_texture_matrix(texture_matrix);

	renderer.set_tint_colour(tint);

	plane.draw();
}

void Blitter::blit(const Texture& tex, glm::vec2 dst_pos, glm::vec4 tint) const
{
	glm::mat4 model_m4 = glm::translate(glm::mat4(1.), glm::vec3(dst_pos, 0))
	                   * glm::scale(glm::mat4(1.), glm::vec3(tex.get_dimensions(), 0));
	glm::mat3 tex_m3(1.);
	blit(tex, model_m4, tex_m3, tint);
}

void Blitter::blit(const Texture& tex, glm::vec2 dst_pos, float angle, glm::vec4 tint) const
{
	glm::mat4 model_m4 = glm::translate(glm::mat4(1.), glm::vec3(dst_pos + tex.get_dimensions()/2.f, 0))
	                   * glm::rotate(glm::mat4(1.), angle, glm::vec3(0, 0, 1))
	                   * glm::translate(glm::mat4(1.), glm::vec3(-tex.get_dimensions()/2.f, 0))
	                   * glm::scale(glm::mat4(1.), glm::vec3(tex.get_dimensions(), 0));
	glm::mat3 tex_m3(1.);
	blit(tex, model_m4, tex_m3, tint);
}

void Blitter::blit(const Texture& tex,
                   glm::vec2 src_pos, glm::vec2 src_dim,
                   glm::vec2 dst_pos, glm::vec2 dst_dim,
                   glm::vec2 center, float angle, glm::vec4 tint) const
{
	glm::mat4 model_m4 = glm::translate(glm::mat4(1.), glm::vec3(dst_pos, 0))
	                   * glm::rotate(glm::mat4(1.), angle, glm::vec3(0, 0, 1))
	                   * glm::translate(glm::mat4(1.), glm::vec3(-center, 0))
	                   * glm::scale(glm::mat4(1.), glm::vec3(dst_dim, 0));
	glm::mat3 tex_m3 = glm::translate(glm::mat4(1.), glm::vec3(src_pos / tex.get_dimensions(), 0))
	                 * glm::scale(glm::mat4(1.), glm::vec3(src_dim / tex.get_dimensions(), 0));
	blit(tex, model_m4, tex_m3, tint);
}

void Blitter::rect_filled(glm::vec2 pos, glm::vec2 dim, glm::vec4 tint, float angle) const
{
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.), glm::vec3(pos + dim/2.f, 0))
	                       * glm::rotate(glm::mat4(1.), angle, glm::vec3(0, 0, 1))
	                       * glm::translate(glm::mat4(1.), glm::vec3(-dim/2.f, 0))
	                       * glm::scale(glm::mat4(1.), glm::vec3(dim, 0));
	renderer.set_model_matrix(model_matrix);

	renderer.set_has_texture(false);
	renderer.set_tint_colour(tint);
	plane.draw();
}

// If drawing lines, Set line thickness before calling this function
void Blitter::stream_vertices(const std::vector<GLfloat>& vertices, GLenum draw_mode, glm::vec4 tint) const
{
	constexpr GLint component_number = 2;
	transfer_geometry<GL_ARRAY_BUFFER, GL_STREAM_DRAW, component_number>(Renderer::vertex_pos_attr_loc, buf_name, vertices);
	glVertexAttrib3f(Renderer::vertex_col_attr_loc, tint.r, tint.g, tint.b); // Set the value used when that vertex attrib is disabled: do not enable!!
	renderer.set_has_texture(false);
	renderer.set_tint_colour(glm::vec4(1.f));

	renderer.set_model_matrix(glm::mat4(1.f));
	glDrawArrays(draw_mode, 0, vertices.size() / component_number);

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glDisableVertexAttribArray(Renderer::vertex_pos_attr_loc);
}

void Blitter::polyline(const vector<GLfloat>& vertices, GLfloat line_thickness, glm::vec4 tint) const
{
	glLineWidth(line_thickness);
	stream_vertices(vertices, GL_LINE_STRIP, tint);
}

void Blitter::polygon(const vector<GLfloat>& vertices, GLfloat line_thickness, glm::vec4 tint) const
{
    glLineWidth(line_thickness);
    stream_vertices(vertices, GL_LINE_LOOP, tint);
}

void Blitter::polygon_filled(const vector<GLfloat>& vertices, glm::vec4 tint) const
{
	// Solution that only works with convex polygons:
    // find the barycenter of all points in vertices, and prepend vertices with the points
	// then render using GL_TRIANGLE_FAN
    polygon(vertices, 1.f, tint); // TODO
}

void Blitter::circle(glm::vec2 center, float radius, GLfloat line_thickness, glm::vec4 tint) const
{
	float prim_gen_number = std::ceil(std::log(radius - 3.f) * 5 + 5);
	float angle = PI_2 / prim_gen_number;
	glm::vec3 base_point(radius, 0., 0.);
	glm::mat3 rot_m(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 0, 1)));

	std::vector<GLfloat> vertices(2 * static_cast<int>(prim_gen_number));
	for (int it=0; it<prim_gen_number; it++) {
		glm::vec3 vertex = (static_cast<float>(it) * rot_m) * base_point;
		vertices[it*2]     = vertex.x;
        vertices[it*2 + 1] = vertex.y;
	}

	// TODO model matrix to translate the circle
	polygon(vertices, line_thickness, tint);
}

void Blitter::disc(glm::vec2 center, float radius, glm::vec4 tint) const
{
}

Blitter::Plane::Plane():
		Static_indexed_mesh({0, 1, 2, 0, 2, 3}, {0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {0, 0, 0, 1, 1, 1, 1, 0})
{}

} // namespace Engine::Renderer
