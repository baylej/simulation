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

class Mesh {
public:
	// Creates GL buffers and loads the data
	Mesh(const vector<GLuint>& indices, const vector<GLfloat>& vertices, const vector<GLfloat>& colors/*, const vector<GLfloat>& uvs*/);
	~Mesh();

	void draw() const;

	constexpr static GLuint prim_type = GL_TRIANGLES;
	constexpr static GLuint prim_component_number = 3;

private:
	// number of element in the indices buffer
	const GLuint indices_size;
	// VBO names
	GLuint indices_buf = GL_NONE;
	GLuint vertices_buf = GL_NONE;
	GLuint colours_buf = GL_NONE;
	//GLuint uvs_buf = GL_NONE;
	// VAO name
	GLuint vao = GL_NONE;
};

#endif //SIMULATION_MESH_HPP
