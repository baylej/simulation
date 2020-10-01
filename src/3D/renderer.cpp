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
#include <array>

#include "utils.hpp"
#include "renderer.hpp"

namespace Engine::N3D {

static constexpr GLchar vert_shader_src[] {
"#version 300 es\n"

"layout(location = 0) in vec3 vx_pos;" // Vertex
"layout(location = 1) in vec3 vx_col;" // Color
//"layout(location = 2) in vec2 vx_uv;"  // UV

"uniform mat4 proj_m4;"
"uniform mat4 view_m4;"
"uniform mat4 model_m4;"

"out vec3 color;"
//"out vec2 uv;"

"void main()"
"{"
"	gl_Position = proj_m4 * view_m4 * model_m4 * vec4(vx_pos, 1.0);"
"	color = vx_col;"
//"	uv = vx_uv;"
"}"};
constexpr GLint vert_shader_src_len = std::size(vert_shader_src);

constexpr GLchar frag_shader_src[] {
"#version 300 es\n"

"out vec4 frag_colour;"

"in vec3 color;"
"in vec2 uv;"

"uniform bool has_tex;"
"uniform sampler2D tex;"
"uniform mat3 tex_trans;"

"void main()"
"{"/*
"	if (has_tex) {"
"		vec3 tex_coord = tex_trans * vec3(uv, 1.);"
"		frag_colour = texture2D(tex, tex_coord.st);"
//	"		frag_colour = vec4(1., 0., 0., 1.0);" // DEBUG
"	}"
"	else {"*/
"		frag_colour = vec4(color, 1.0);"
//"	}"
"}"};
constexpr GLint frag_shader_src_len = std::size(frag_shader_src);

constexpr GLsizei log_buf_size = 4096;

GLuint load_shader(const GLchar* source, GLenum type, GLint source_len)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, &source_len);
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

Renderer::Renderer()
{
	GLuint vert_shader = load_shader(static_cast<const GLchar*>(vert_shader_src), GL_VERTEX_SHADER, vert_shader_src_len);
	GLuint frag_shader = load_shader(static_cast<const GLchar*>(frag_shader_src), GL_FRAGMENT_SHADER, frag_shader_src_len);

	program = load_program({vert_shader, frag_shader});

	// Flag shaders for deletion
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	check_gl_error("Renderer::ctor"s);
}

Renderer::~Renderer() {
	glDeleteProgram(program);
}

} // namespace Engine::N3D
