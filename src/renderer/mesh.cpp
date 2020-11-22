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

	transfer_geometry<GL_ARRAY_BUFFER, GL_STATIC_DRAW>(Renderer::vertex_pos_attr_loc, vertices_buf, vertices);

	if (colours_buf) {
        transfer_geometry<GL_ARRAY_BUFFER, GL_STATIC_DRAW>(Renderer::vertex_col_attr_loc, colours_buf, colours);
    }

	if (uvs_buf) {
        transfer_geometry<GL_ARRAY_BUFFER, GL_STATIC_DRAW, 2>(Renderer::vertex_uv_attr_loc, uvs_buf, uvs);
	}

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE); // DO NOT UNBIND !!

	glBindVertexArray(GL_NONE);

	check_gl_error("Mesh::ctor");
}

Static_indexed_mesh::~Static_indexed_mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &indices_buf);
	glDeleteBuffers(1, &colours_buf);
	glDeleteBuffers(1, &vertices_buf);
	check_gl_error("Mesh::dtor");
}

void Static_indexed_mesh::draw(GLenum prim_type) const
{
	glBindVertexArray(vao);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf); // Already bound in constructor
	glDrawElements(prim_type, indices_size, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(GL_NONE);

#ifndef NDEBUG
	check_gl_error("Mesh::draw");
#endif
}

Triangle::Triangle():
		Static_indexed_mesh({0, 1, 2}, {0, .5, 0, .5, -.5, 0, -.5, -.5, 0}, {1, 0, 0, 0, 1, 0, 0, 0, 1}, {})
{}

const Triangle& Triangle::get()
{
	static Triangle instance;
	return instance;
}

Plane::Plane():
		Static_indexed_mesh({0, 1, 2, 0, 2, 3}, {-1, -1, 0, -1, 1, 0, 1, 1, 0, 1, -1, 0}, {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0, 1, 1, 1, 1, 0})
{}

const Plane& Plane::get()
{
	static Plane plane;
	return plane;
}

Cube::Cube():
		Static_indexed_mesh(
			{
                // Face front
                0,  1,  2,   0,  2,  3,
                // Face back
                4,  5,  6,   4,  6,  7,
                // Face left
                8,  9, 10,   8, 10, 11,
                // Face right
                12, 13, 14,  12, 14, 15,
                // Face top
                16, 17, 18,  16, 18, 19,
                // Face down
                20, 21, 22,  20, 22, 23
            },
			{
				-0.5, -0.5,  0.5,   0.5, -0.5,  0.5,   0.5,  0.5,  0.5,  -0.5,  0.5,  0.5,
				-0.5, -0.5, -0.5,  -0.5,  0.5, -0.5,   0.5,  0.5, -0.5,   0.5, -0.5, -0.5,
				-0.5, -0.5, -0.5,  -0.5, -0.5,  0.5,  -0.5,  0.5,  0.5,  -0.5,  0.5, -0.5,
				 0.5, -0.5,  0.5,   0.5, -0.5, -0.5,   0.5,  0.5, -0.5,   0.5,  0.5,  0.5,
				-0.5,  0.5,  0.5,   0.5,  0.5,  0.5,   0.5,  0.5, -0.5,  -0.5,  0.5, -0.5,
				-0.5, -0.5,  0.5,  -0.5, -0.5, -0.5,   0.5, -0.5, -0.5,   0.5, -0.5,  0.5
			},
			{
				1., .9, 0.,  1., .9, 0.,  1., .9, 0.,  1., .9, 0.,
				.7, .8, .9,  .7, .8, .9,  .7, .8, .9,  .7, .8, .9,
				1., .7, .8,  1., .7, .8,  1., .7, .8,  1., .7, .8,
				.7, 1., .9,  .7, 1., .9,  .7, 1., .9,  .7, 1., .9,
				0., 1., .5,  0., 1., .5,  0., 1., .5,  0., 1., .5,
				.5, .8, .9,  .5, .8, .9,  .5, .8, .9,  .5, .8, .9
			},
			{
				0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0,
				1.0, 0.0,  0.0, 0.0,  0.0, 1.0,  1.0, 1.0,
				1.0, 0.0,  0.0, 0.0,  0.0, 1.0,  1.0, 1.0,
				1.0, 0.0,  0.0, 0.0,  0.0, 1.0,  1.0, 1.0,
				0.0, 1.0,  1.0, 1.0,  1.0, 0.0,  0.0, 0.0,
				0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0
			}
		)
{}

const Cube& Cube::get()
{
	static Cube cube;
	return cube;
}

}
