/*
    3D Physics Simulations - Mesh: Indexed mesh class
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

#ifndef SIMULATION_MESH_HPP
#define SIMULATION_MESH_HPP

#include <GLES3/gl3.h>

#include <vector>

using std::vector;

namespace Engine::Renderer {

// Immutable class
class Static_indexed_mesh {
public:
	// Creates a VAO and Buffers and loads the data
	Static_indexed_mesh(const vector<GLuint>& indices, const vector<GLfloat>& vertices, const vector<GLfloat>& colors, const vector<GLfloat>& uvs);
	// Delete the VAO and Buffers
	~Static_indexed_mesh();

	// Delete copy and move, all implementations are singletons
	Static_indexed_mesh(const Static_indexed_mesh&) = delete;
	Static_indexed_mesh(Static_indexed_mesh&&) = delete;
	Static_indexed_mesh& operator=(const Static_indexed_mesh& other) = delete;
	Static_indexed_mesh& operator=(Static_indexed_mesh&& other) = delete;

	// Indices are already bound in the returned VAO, you probably want to use member function `draw`
	[[nodiscard]] GLuint get_vao() const { return vao; }
	// Draw mesh,
	void draw(GLenum type = GL_TRIANGLES) const;

	constexpr static GLuint vertex_component_number = 3;
	constexpr static GLuint colour_component_number = 3;
	constexpr static GLuint uv_component_number = 2;

private:
	// number of element in the indices buffer
	const GLuint indices_size;
	// VBO names
	const GLuint indices_buf;
	const GLuint vertices_buf;
	const GLuint colours_buf;
	const GLuint uvs_buf;
	// VAO name
	const GLuint vao;
};

class Triangle : public Static_indexed_mesh {
public:
	Triangle(const Triangle&) = delete;
	Triangle(Triangle&&) = delete;
	Triangle &operator=(const Triangle&) = delete;
	Triangle &operator=(Triangle&&) = delete;

	// Lazy initialised singleton
	static const Triangle& get();

private:
	Triangle();
	~Triangle() = default;
};

}

#endif //SIMULATION_MESH_HPP
