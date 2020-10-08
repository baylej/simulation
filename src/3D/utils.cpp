/*
    3D Physics Simulations - Utils: various utility functions and classes
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
#include "utils.hpp"

#include <iostream>

constexpr GLsizei log_buf_size = 4096;

namespace Engine::N3D {

GLuint load_shader(const GLchar* source, GLenum type, GLint source_len)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, &source_len);
	glCompileShader(shader);
	GLint compile_status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		GLchar log[log_buf_size];
		GLsizei log_len = 0;
		glGetShaderInfoLog(shader, log_buf_size, &log_len, static_cast<GLchar*>(log));
		glDeleteShader(shader);
		throw std::runtime_error("GL Error: Could not compile shader, "s + static_cast<char*>(log));
	}
	return shader;
}

// Create a program from all the supplied shaders
GLuint load_program(std::initializer_list<GLuint> shaders)
{
	GLuint program = glCreateProgram();
	for (GLuint it: shaders) {
		glAttachShader(program, it);
	}
	glLinkProgram(program);

	GLint link_status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	GLint validate_status = GL_FALSE;
	if (link_status == GL_TRUE) {
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &validate_status);
	}

	if (link_status == GL_FALSE || validate_status == GL_FALSE) {
		GLchar log[log_buf_size];
		GLsizei log_len = 0;
		glGetProgramInfoLog(program, log_buf_size, &log_len, static_cast<GLchar*>(log));
		glDeleteProgram(program);
		throw std::runtime_error("GL Error: Could not link/validate program, "s + static_cast<char*>(log));
	}
	return program;
}

GLint get_check_uniform(GLuint program, const GLchar* uniform_name)
{
	GLint res = glGetUniformLocation(program, uniform_name);
	if (res == -1) {
		throw std::runtime_error("GL Error: uniform location not found: "s + uniform_name);
	}
	return res;
}

void list_uniforms(GLuint program)
{
	GLint count = GL_ZERO;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	std::cerr << "active uniforms: " << count << std::endl;

	GLchar buf[64];
	GLint size = GL_ZERO;
	GLenum type = GL_NONE;
	for (GLint it=0; it<count; it++) {
		glGetActiveUniform(program, static_cast<GLuint>(it), sizeof(buf), NULL, &size, &type, static_cast<GLchar*>(buf));
		std::cerr << "uniform " << static_cast<const GLchar*>(buf) << " of type " << type << std::endl;
	}

}

} // namespace Engine::N3D
