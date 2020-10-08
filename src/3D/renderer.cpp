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
#include "utils.hpp"
#include "renderer.hpp"

#include <array>

#include <glm/gtc/type_ptr.hpp>

namespace Engine::N3D {

static constexpr GLchar vert_shader_src[] { // FIXME use modern C++ raw string literals
"#version 300 es\n"

"layout(location = 0) in highp vec3 vx_pos;" // Vertex
"layout(location = 1) in highp vec3 vx_col;" // Color
"layout(location = 2) in vec2 vx_uv;"  // UV

"uniform highp mat4 proj_m4;"
"uniform highp mat4 view_m4;"
"uniform highp mat4 model_m4;"

"out highp vec3 color;"
"out highp vec2 uv;"

"void main()"
"{"
"	gl_Position = proj_m4 * view_m4 * model_m4 * vec4(vx_pos, 1.0);"
"	color = vx_col;"
"	uv = vx_uv;"
"}"};
constexpr GLint vert_shader_src_len = std::size(vert_shader_src);

constexpr GLchar frag_shader_src[] { // FIXME use modern C++ raw string literals
"#version 300 es\n"

"out highp vec4 frag_colour;"

"in highp vec3 color;"
"in highp vec2 uv;"

"uniform bool has_tex;"
"uniform sampler2D tex;"
"uniform highp mat3 tex_m3;"

"void main()"
"{"/*
"	if (has_tex) {"
"		vec3 tex_coord = tex_m3 * vec3(uv, 1.);"
"		frag_colour = texture2D(tex, tex_coord.st);"
//	"		frag_colour = vec4(1., 0., 0., 1.0);" // DEBUG
"	}"
"	else {"*/
"		frag_colour = vec4(color, 1.0);"
//"	}"
"}"};
constexpr GLint frag_shader_src_len = std::size(frag_shader_src);

Renderer::Renderer()
{
	GLuint vert_shader = load_shader(static_cast<const GLchar*>(vert_shader_src), GL_VERTEX_SHADER, vert_shader_src_len);
	GLuint frag_shader = load_shader(static_cast<const GLchar*>(frag_shader_src), GL_FRAGMENT_SHADER, frag_shader_src_len);

	program = load_program({vert_shader, frag_shader});

	// Flag shaders for deletion
	glDetachShader(program, vert_shader);
	glDetachShader(program, frag_shader);
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	check_gl_error("Renderer::ctor#1"s);

	// If there is a uniform that you are not using, the driver will optimize your uniform out.
	// Then `glGetUniformLocation` returns -1.

	// Uniform locations in the vertex shader
	proj_m4_loc = get_check_uniform(program, "proj_m4");
	view_m4_loc = get_check_uniform(program, "view_m4");
	model_m4_loc = get_check_uniform(program, "model_m4");

	// Uniform locations in the fragment shader
	/*has_tex_loc = get_check_uniform(program, "has_tex");
	tex_loc = get_check_uniform(program, "tex");
	tex_m3_loc = get_check_uniform(program, "tex_m3");*/

	check_gl_error("Renderer::ctor#2"s);
}

Renderer::~Renderer() {
	glDeleteProgram(program);
}

void Renderer::set_proj_view_matrices(const Camera &camera) const
{
	glUniformMatrix4fv(proj_m4_loc, 1, GL_FALSE, glm::value_ptr(camera.get_proj_matrix()));
	glUniformMatrix4fv(view_m4_loc, 1, GL_FALSE, glm::value_ptr(camera.get_view_matrix()));
}

void Renderer::set_model_matrix(const glm::mat4 &model_m4) const
{
	glUniformMatrix4fv(model_m4_loc, 1, GL_FALSE, glm::value_ptr(model_m4));
}

} // namespace Engine::N3D
