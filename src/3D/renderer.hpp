/*
    3D Physics Simulations - Renderer: 3D Renderer (OpenGL ES 3)
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

#ifndef SIMULATION_RENDERER_HPP
#define SIMULATION_RENDERER_HPP

#include <GLES3/gl3.h>

namespace Engine::N3D {

class Renderer {
public:
	// Compiles shaders, link program, use program
	Renderer();
	~Renderer();
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) = default;

	// Location of vertex attributes defined in the vertex shader
	constexpr static GLuint vertex_pos_attr_loc = 0;
	constexpr static GLuint vertex_col_attr_loc = 1;

	[[nodiscard]] GLuint get_program_name() const { return program; };

private:
	GLuint program;
};

} // namespace Engine::N3D

#endif //SIMULATION_RENDERER_HPP
