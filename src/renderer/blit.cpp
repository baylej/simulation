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

	glDrawArrays(draw_mode, 0, vertices.size() / component_number);

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glDisableVertexAttribArray(Renderer::vertex_pos_attr_loc);
}

void Blitter::polyline(const vector<GLfloat>& vertices, GLfloat line_thickness, glm::vec4 tint) const
{
	glLineWidth(line_thickness);
	renderer.set_model_matrix(glm::mat4(1.f));
	stream_vertices(vertices, GL_LINE_STRIP, tint);
}

void Blitter::polygon(const vector<GLfloat>& vertices, GLfloat line_thickness, glm::vec4 tint) const
{
	glLineWidth(line_thickness);
	renderer.set_model_matrix(glm::mat4(1.f));
	stream_vertices(vertices, GL_LINE_LOOP, tint);
}

void Blitter::polygon_filled(const vector<GLfloat>& vertices, glm::vec4 tint) const
{
	// Solution that only works with convex polygons:
	// find the barycenter of all points in vertices, and prepend vertices with the points
	// then render using GL_TRIANGLE_FAN
	renderer.set_model_matrix(glm::mat4(1.f));
	polygon(vertices, 1.f, tint); // TODO
}

// generate `vertices_number` vertices around (0, 0), radius=1.0; `vertices` must be pre-allocated
// STRIDE: how many float to skip at the begin of vertices
// CLOSE: close the circle: adds 2 extra floats at the end
//   (same coords as the first generated vertex, vertices.size must be at least `vertices_number + 2`)
template<int STRIDE = 0, bool CLOSE = false>
static constexpr inline void generate_circle_vertices(std::vector<GLfloat>& vertices, int vertices_number, float angle)
{
	int it = 0;
	for (; it<vertices_number; it++) {
		vertices[STRIDE + it*2]     = cosf(angle * static_cast<float>(it));
		vertices[STRIDE + it*2 + 1] = sinf(angle * static_cast<float>(it));
	}
	if (CLOSE) {
		vertices[STRIDE + it*2]     = vertices[STRIDE];
		vertices[STRIDE + it*2 + 1] = vertices[STRIDE + 1];
	}
}

static constexpr inline void ideal_vertices_for_radius(float radius, int* prim_gen_number, float* angle)
{
	assert(radius > 0.f);
	*prim_gen_number = std::ceil(std::log(radius - 3.f) * 10 + 5);
	*angle = PI_2 / *prim_gen_number;
}

void Blitter::circle(glm::vec2 center, float radius, GLfloat line_thickness, glm::vec4 tint) const
{
	int vx_count;
	float angle;
	ideal_vertices_for_radius(radius, &vx_count, &angle);
	std::vector<GLfloat> vertices(2 * vx_count);
	generate_circle_vertices(vertices, vx_count, angle);

	renderer.set_model_matrix(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(center, 0.f)), glm::vec3(radius, radius, 1.f)));
	glLineWidth(line_thickness);
	stream_vertices(vertices, GL_LINE_LOOP, tint);
}

void Blitter::disc(glm::vec2 center, float radius, glm::vec4 tint) const
{
	int vx_count;
	float angle;
	ideal_vertices_for_radius(radius, &vx_count, &angle);
	std::vector<GLfloat> vertices(2 * vx_count + 2 + 2); // + STRIDE + CLOSE
	vertices[0] = vertices[1] = 0.f;
	generate_circle_vertices<2, true>(vertices, vx_count, angle);

	renderer.set_model_matrix(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(center, 0.f)), glm::vec3(radius, radius, 1.f)));
	stream_vertices(vertices, GL_TRIANGLE_FAN, tint);
}

Blitter::Plane::Plane():
		Static_indexed_mesh({0, 1, 2, 0, 2, 3}, {0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {0, 0, 0, 1, 1, 1, 1, 0})
{}

} // namespace Engine::Renderer
