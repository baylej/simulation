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
#include <glm/ext/matrix_clip_space.hpp>

using namespace glm;

namespace Engine::Renderer {
Camera2D::Camera2D(unsigned int display_height, unsigned int display_width)
{
	update_display_dimensions(display_height, display_width);
}

void Camera2D::update_display_dimensions(unsigned int display_height, unsigned int display_width)
{
	proj_matrix = translate(mat4(1.f), vec3(-1.f, -1.f, 0)) * scale(mat4(1.f), vec3(2.f, 2.f, 0));
	view_matrix = scale(mat4(1.f), vec3(1/static_cast<float>(display_width), 1/static_cast<float>(display_height), 1.f));
}

Camera3D::Camera3D()
{
	position = glm::vec3(0.f);
	sight = glm::vec3(0.f, 0.f, -1.f);
}

void Camera3D::set_perspective_projection(float fov_y, float aspect_ratio, float z_near, float z_far)
{
	proj_matrix = glm::perspective(fov_y, aspect_ratio, z_near, z_far);
	// Alternatives:
	// - perspectiveFov (T fov, T width, T height, T near, T far)
	// - ortho (T left, T right, T bottom, T top, T zNear, T zFar)
	// - infinitePerspective (T fovy, T aspect, T near)
	// - frustum (T left, T right, T bottom, T top, T near, T far)
	// - tweakedInfinitePerspective (T fovy, T aspect, T near)

	// Suffixes:
	// - ZO : The near and far clip planes correspond to z normalized device coordinates of  0 and +1 respectively. (Direct3D clip volume definition)
	// - NO : The near and far clip planes correspond to z normalized device coordinates of -1 and +1 respectively. (OpenGL clip volume definition)
}

void Camera3D::update_camera()
{
	view_matrix = glm::lookAt(position, sight, glm::vec3(0, 1, 0));
}

} // namespace Engine::Renderer
