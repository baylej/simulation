/*
    3D Physics Simulations - Camera: perspective and orthographic camera
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
#include "camera.hpp"

#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace Engine::Renderer {
Camera2D::Camera2D(unsigned int display_height, unsigned int display_width)
{
	update_display_dimensions(display_height, display_width);
}

void Camera2D::update_display_dimensions(unsigned int display_height, unsigned int display_width)
{
	proj_matrix = mat4(1.f);
	proj_matrix[2].z = 0.f;
	view_matrix = scale(translate(mat4(1.f), vec3(-1, -1, 0)), vec3(2/static_cast<float>(display_width), 2/static_cast<float>(display_height), 1));
}

} // namespace Engine::Renderer
