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
#include <glm/mat4x4.hpp>

namespace Engine::Renderer {

class Renderer {
public:
	// Compiles shaders, link program
	Renderer();
	// Deletes the program (hence this object is movable but not copyable)
	~Renderer();

	// Not copyable, not copy assignable
	// Movable, move assignable
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) = default;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = default;

	// Location of vertex attributes defined in the vertex shader
	constexpr static GLuint vertex_pos_attr_loc = 0;
	constexpr static GLuint vertex_col_attr_loc = 1;
	constexpr static GLuint vertex_uv_attr_loc  = 2;
	constexpr static GLuint texture_unit_name = 0; // GL_TEXTURE0 + 0 is bound

	// glUseProgram this renderer's program
	void use_program() const { glUseProgram(program); glUniform1i(tex_loc, texture_unit_name); };

	// To use the following functions, this renderer's program must be in use
	void set_proj_matrix(const glm::mat4& proj_m4) const;
	void set_view_matrix(const glm::mat4& view_m4) const;
	void set_model_matrix(const glm::mat4& model_m4) const;
	void set_has_texture(bool has_tex) const;
	void set_texture_matrix(const glm::mat3& tex_m3) const;

private:
	GLuint program;

	// Location of uniforms
	// - Vertex shader
	GLint proj_m4_loc;
	GLint view_m4_loc;
	GLint model_m4_loc;
	// - Fragment shader
	GLint has_tex_loc;
	GLint tex_loc;
	GLint tex_m3_loc;
};

} // namespace Engine::N3D

#endif //SIMULATION_RENDERER_HPP
