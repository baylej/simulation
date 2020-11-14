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

#include <stdexcept>
#include <glm/ext/matrix_transform.hpp>

namespace Engine::Renderer {

Blitter::Blitter(const Renderer& renderer) :
		renderer(renderer), plane()
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

Blitter::Plane::Plane():
		Static_indexed_mesh({0, 1, 2, 0, 2, 3}, {0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {0, 0, 0, 1, 1, 1, 1, 0})
{}


} // namespace Engine::Renderer
