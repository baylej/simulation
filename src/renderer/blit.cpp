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

Blitter::Blitter(const Renderer& renderer, const Plane& plane) :
		renderer(renderer), plane(plane)
{}

void Blitter::blit(GLuint tex_name,
		float src_x, float src_y, float src_w, float src_h,
		float dst_x, float dst_y, float dst_w, float dst_h) const
{
	//glActiveTexture(GL_TEXTURE0 + Renderer::texture_unit_name);
	//glBindTexture(GL_TEXTURE_2D, tex_name);
	renderer.set_model_matrix(glm::scale(glm::translate(glm::mat4(1.), glm::vec3(dst_x, dst_y, 0)), glm::vec3(dst_w/2., dst_h/2., 0)));check_gl_error("Blitter::blit 1");
	renderer.set_has_texture(false);check_gl_error("Blitter::blit 2");
	renderer.set_texture_matrix(glm::scale(glm::translate(glm::mat4(1.), glm::vec3(src_x, src_y, 0)), glm::vec3(src_w/2., src_h/2., 0)));
	check_gl_error("Blitter::blit 3");
	plane.draw();
}

} // namespace Engine::Renderer
