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
	Camera();
	~Camera() = default;

	[[nodiscard]] const glm::mat4& get_proj_matrix() const
	{
		return proj_matrix;
	}

	[[nodiscard]] const glm::mat4& get_view_matrix() const
	{
		return view_matrix;
	}

private:
	glm::mat4 proj_matrix;
	glm::mat4 view_matrix;
};

} // namespace Engine::N3D

#endif //SIMULATION_CAMERA_HPP
