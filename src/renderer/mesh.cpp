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

#include <iostream>

namespace Engine::Renderer {

template<void (*G)(int, GLuint*)>
GLuint generateArray(bool generate = true)
{
	GLuint name = GL_NONE;
	if (generate) {
		G(1, &name);
	}
	return name;
}

Static_indexed_mesh::Static_indexed_mesh(const vector<GLuint>& indices, const vector<GLfloat>& vertices, const vector<GLfloat>& colours, const vector<GLfloat>& uvs):
		indices_size(indices.size()),
		indices_buf(generateArray<glGenBuffers>()),
		vertices_buf(generateArray<glGenBuffers>()),
		colours_buf(generateArray<glGenBuffers>(!colours.empty())),
		uvs_buf(generateArray<glGenBuffers>(!uvs.empty())),
		vao(generateArray<glGenVertexArrays>())
{
	glBindVertexArray(vao);

	glEnableVertexAttribArray(Renderer::vertex_pos_attr_loc);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_buf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(Renderer::vertex_pos_attr_loc, vertex_component_number, GL_FLOAT, GL_FALSE, 0, nullptr);

	if (colours_buf) {
		glEnableVertexAttribArray(Renderer::vertex_col_attr_loc);
		glBindBuffer(GL_ARRAY_BUFFER, colours_buf);
		glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(GLfloat), colours.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(Renderer::vertex_col_attr_loc, colour_component_number, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	if (uvs_buf) {
		glEnableVertexAttribArray(Renderer::vertex_uv_attr_loc);
		glBindBuffer(GL_ARRAY_BUFFER, uvs_buf);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(Renderer::vertex_uv_attr_loc, uv_component_number, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE); // DO NOT UNBIND !!

	glBindVertexArray(GL_NONE);

	check_gl_error("Mesh::ctor"s);
}

Static_indexed_mesh::~Static_indexed_mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &indices_buf);
	glDeleteBuffers(1, &colours_buf);
	glDeleteBuffers(1, &vertices_buf);
	check_gl_error("Mesh::dtor"s);
}

void Static_indexed_mesh::draw(GLenum prim_type) const
{
	glBindVertexArray(vao);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf); // Already bound in constructor
	glDrawElements(prim_type, indices_size, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(GL_NONE);

#ifndef NDEBUG
	check_gl_error("Mesh::draw"s);
#endif
}

Triangle::Triangle():
		Static_indexed_mesh({0, 1, 2}, {0, .5, 0, .5, -.5, 0, -.5, -.5, 0}, {1, 0, 0, 0, 1, 0, 0, 0, 1}, {})
{}

const Triangle &Triangle::get()
{
	static Triangle instance;
	return instance;
}

}
