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
#pragma once

#ifndef SIMULATION_CAMERA_HPP
#define SIMULATION_CAMERA_HPP

#include <glm/mat4x4.hpp>

namespace Engine::Renderer {

class Camera {
public:

	[[nodiscard]] const glm::mat4& get_proj_matrix() const
	{
		return proj_matrix;
	}

	[[nodiscard]] const glm::mat4& get_view_matrix() const
	{
		return view_matrix;
	}
protected:
	glm::mat4 proj_matrix;
	glm::mat4 view_matrix;
};

// 2D camera, translates absolute screen coordinates in pixel (Origin is bottom-left) to OpenGL screen coordinates (-1,+1)
class Camera2D: public Camera {
public:
	Camera2D(unsigned int display_height, unsigned int display_width);
	~Camera2D() = default;
	Camera2D(const Camera2D&) = default;
	Camera2D(Camera2D&&) = default;
	Camera2D& operator=(const Camera2D&) = default;
	Camera2D& operator=(Camera2D&&) = default;

	// Update this camera's view and projection matrices
	void update_display_dimensions(unsigned int display_height, unsigned int display_width);
};

class Camera3D: public Camera {
public:
	Camera3D();
	~Camera3D() = default;
	Camera3D(const Camera3D&) = default;
	Camera3D(Camera3D&&) = default;
	Camera3D& operator=(const Camera3D&) = default;
	Camera3D& operator=(Camera3D&&) = default;

	void set_pos(const glm::vec3& pos)
	{
		position = pos;
	}
	void set_sight(const glm::vec3& sight)
	{
		this->sight = sight;
	}
	// TODO functions to move the camera along its local orientation (FPS camera)

	// Update this camera's projection matrix
	void set_perspective_projection(float fov_y = /* PI/3 */ 1.0471975511965976, float aspect_ratio = 16 / 9.f, float z_near = .1f, float z_far = 100.f);
	// Update this camera's view and matrix from its position and sight vector
	// should be called only once per frame
	void update_camera();

protected:
	glm::vec3 position;
	glm::vec3 sight;
};

} // namespace Engine::N3D

#endif //SIMULATION_CAMERA_HPP
