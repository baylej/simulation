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
#include "mesh.hpp"
#include "utils.hpp"
#include "renderer.hpp"

Mesh::Mesh(const vector<GLuint>& indices, const vector<GLfloat>& vertices, const vector<GLfloat>& colours/*, const vector<GLfloat>& uvs*/):
		indices_size(indices.size())
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertices_buf);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_buf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::vertex_pos_attr_loc, prim_component_number, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &colours_buf);
	glBindBuffer(GL_ARRAY_BUFFER, colours_buf);
	glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(GLfloat), colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::vertex_col_attr_loc, prim_component_number, GL_FLOAT, GL_FALSE, 0, nullptr);

	/*glGenBuffers(1, &uvs_buf);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_buf);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);*/

	glGenBuffers(1, &indices_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(GL_NONE);

	check_gl_error("Mesh::ctor"s);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &indices_buf);
	glDeleteBuffers(1, &colours_buf);
	glDeleteBuffers(1, &vertices_buf);
	check_gl_error("Mesh::dtor"s);
}

void Mesh::draw() const
{
	glBindVertexArray(vao);

	//glBindBuffer(GL_ARRAY_BUFFER, vertices_buf); // FIXME: already bound in the constructor?
	glDrawElements(prim_type, indices_size, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(GL_NONE);

	check_gl_error("Mesh::draw"s);
}
