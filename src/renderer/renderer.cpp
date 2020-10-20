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

namespace Engine::Renderer {

static constexpr const GLchar* vert_shader_src =
R"shader(#version 300 es

layout(location = 0) in highp vec3 vx_pos; // Vertex
layout(location = 1) in highp vec3 vx_col; // Color
layout(location = 2) in vec2 vx_uv; // UV

uniform highp mat4 proj_m4;
uniform highp mat4 view_m4;
uniform highp mat4 model_m4;

out highp vec3 colour;
out highp vec2 uv;

void main()
{
	gl_Position = proj_m4 * view_m4 * model_m4 * vec4(vx_pos, 1.0);
	colour = vx_col;
	gl_PointSize = 5.;
	uv = vx_uv;
})shader";

static constexpr const GLchar* frag_shader_src =
R"shader(#version 300 es

out highp vec4 frag_colour;

in highp vec3 colour;
in highp vec2 uv;

uniform bool has_tex;
uniform sampler2D tex;
uniform highp mat3 tex_m3;

void main()
{
	if (has_tex) {
		highp vec3 tex_coord = tex_m3 * vec3(uv, 1.);
		frag_colour = texture(tex, tex_coord.st);
		// DEBUG to see UV values as RG colors (texture lookup because the shader compiler optimisation removes unused uniforms)
//		frag_colour = vec4(tex_coord.st, 0., texture(tex, tex_coord.st).a);
	}
	else {
		frag_colour = vec4(colour, 1.0);
	}
})shader";

Renderer::Renderer()
{
	GLuint vert_shader = load_shader(static_cast<const GLchar*>(vert_shader_src), GL_VERTEX_SHADER);
	GLuint frag_shader = load_shader(static_cast<const GLchar*>(frag_shader_src), GL_FRAGMENT_SHADER);

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
	has_tex_loc = get_check_uniform(program, "has_tex");
	tex_loc = get_check_uniform(program, "tex");
	tex_m3_loc = get_check_uniform(program, "tex_m3");

	check_gl_error("Renderer::ctor#2"s);
}

Renderer::~Renderer() {
	glDeleteProgram(program);
}

void Renderer::set_proj_view_matrices(const Camera2D &camera) const
{
	glUniformMatrix4fv(proj_m4_loc, 1, GL_FALSE, glm::value_ptr(camera.get_proj_matrix()));
	glUniformMatrix4fv(view_m4_loc, 1, GL_FALSE, glm::value_ptr(camera.get_view_matrix()));
}

void Renderer::set_model_matrix(const glm::mat4 &model_m4) const
{
	glUniformMatrix4fv(model_m4_loc, 1, GL_FALSE, glm::value_ptr(model_m4));
}

void Renderer::set_has_texture(bool has_tex) const
{
	glUniform1i(has_tex_loc, has_tex ? GL_TRUE : GL_FALSE);
}

void Renderer::set_texture_matrix(const glm::mat3& tex_m3) const
{
	glUniformMatrix3fv(tex_m3_loc, 1, GL_FALSE, glm::value_ptr(tex_m3));
}

} // namespace Engine::N3D
