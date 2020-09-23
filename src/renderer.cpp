/*
    3D Physics Simulations - Renderer: 3D Renderer
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
#include <exception>
#include <array>
#include "main.hpp"
#include "renderer.hpp"

static constexpr GLchar vert_shader_src[] {
"#version 100\n"

"attribute vec3 vx_pos;" // Vertice
"attribute vec3 vx_col;" // Color
"attribute vec2 vx_uv;"  // UV
"attribute float vx_hastx;"

"uniform mat4 proj_m4;"
"uniform mat4 view_m4;"
"uniform mat4 model_m4;"

"varying vec3 color;"
"varying vec2 uv;"
"varying float has_tex;"

"void main()"
"{"
"	gl_Position = proj_m4 * view_m4 * model_m4 * vec4(vx_pos, 1.0);"
"	color = vx_col;"
"	uv = vx_uv;"
"	has_tex = vx_hastx;"
"}"};
constexpr GLint vert_shader_src_len = std::size(vert_shader_src);

constexpr GLchar frag_shader_src[] {
"#version 100\n"

"precision mediump int;"
"precision mediump float;"

"varying vec3 color;\n"
"varying vec2 uv;"
"varying float has_tex;"

"uniform sampler2D tex;"
"uniform mat3 tex_trans;"

"void main()\n"
"{"
"	if (has_tex > 0.) {"
"		vec3 tex_coord = tex_trans * vec3(uv, 1.);"
"		gl_FragColor = texture2D(tex, tex_coord.st);"
//	"		gl_FragColor = vec4(1., 0., 0., 1.0);" // DEBUG
"	}"
"	else {"
"		gl_FragColor = vec4(color, 1.0);"
"	}"
"}"};
constexpr GLint frag_shader_src_len = std::size(frag_shader_src);

GLuint load_shader(const GLchar* source, GLenum type, GLint source_len)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, &source_len);
	GLint compile_status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		glDeleteShader(shader);
		// TODO glGetShaderInfoLog()
		// TODO throw
		std::terminate(); // DELME
	}
	return shader;
}

Renderer::Renderer()
{
	GLuint program = glCreateProgram();
	GLuint vert_shader = load_shader(static_cast<const GLchar*>(vert_shader_src), GL_VERTEX_SHADER, vert_shader_src_len);
	GLuint frag_shader = load_shader(static_cast<const GLchar*>(frag_shader_src), GL_FRAGMENT_SHADER, frag_shader_src_len);
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	GLint link_status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		// TODO info
		// TODO throw
		std::terminate();
	}
}
