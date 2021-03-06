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
#pragma once

#ifndef SIMULATION_UTILS_HPP
#define SIMULATION_UTILS_HPP

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <GLES3/gl3.h>

constexpr float PI         = 3.141592653589793f; // acos(-1.f)
constexpr float PI_2       = 6.283185307179586f; // 2*acos(-1)
constexpr float PI_HALF    = 1.5707963267948966; // acos(-1)/2.f
constexpr float PI_THIRD   = 1.0471975511965976; // acos(-1)/3.f
constexpr float PI_QUARTER = 0.7853981633974483; // acos(-1)/4.f
constexpr float PI_SIXTH   = 0.5235987755982988; // acos(-1)/6.f
constexpr float PI_2THIRD  = 2.0943951023931953; // 2*acos(-1)/3.f

namespace Engine::Renderer {

// Checks whether an error occurred in the GL calls performed until now
// throws a runtime_exception
inline void check_gl_error(const std::string& where)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::stringstream msg;
		msg << "GL Error: ";
		switch (error) {
			case GL_INVALID_ENUM:                  msg << "invalid enum value";                   break;
			case GL_INVALID_VALUE:                 msg << "invalid argument";                     break;
			case GL_INVALID_OPERATION:             msg << "operation not allowed";                break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: msg << "framebuffer operation not allowed";    break;
			case GL_OUT_OF_MEMORY:                 msg << "OUT OF MEMORY";                        break; // this error is fatal to the GL state
			default:                               msg << "unknown error: " << error;
		}
		msg << ", at " << where;
		throw std::runtime_error(msg.str());
	}
}

// Loads the given Shader from source, and return its GL name
// throws a runtime_exception on failure
GLuint load_shader(const GLchar* source, GLenum type);

// Links all the given shaders into a program and return its GL name
// throws a runtime_exception on failure
GLuint load_program(std::initializer_list<GLuint> shaders);

// Get the location of a named uniform in a program
// throws a runtime_exception if not found
GLint get_check_uniform(GLuint program, const GLchar* uniform_name);

// List uniform names on std::cerr, for debugging purposes
void list_uniforms(GLuint program);

// Generates a single Buffer or VAO name (does nothing and returns GL_NONE if parameter==false)
// Template parameter should be either glGenBuffers or glGenVertexArrays
template<void (*G)(int, GLuint*)>
GLuint generateArray(bool generate = true)
{
	GLuint name = GL_NONE;
	if (generate) {
		G(1, &name);
	}
	return name;
}

// Enables vextex_attrib_loc + binds and send data into target_buf + VertexAttribdata (to be used with a VAO, or DrawArray)
template<GLuint TargetT, GLuint UsageT, GLint CompNumber = 3, GLboolean Normalized = GL_FALSE, typename GlT = GLfloat, GLuint DataT = GL_FLOAT>
void transfer_geometry(GLuint vextex_attrib_loc, GLuint target_buf, const std::vector<GlT>& data, GLsizei stride = 0, const void* pointer = nullptr)
{
    glEnableVertexAttribArray(vextex_attrib_loc);
    glBindBuffer(TargetT, target_buf);
    glBufferData(TargetT, data.size() * sizeof(GlT), data.data(), UsageT);
    glVertexAttribPointer(vextex_attrib_loc, CompNumber, DataT, Normalized, stride, pointer);
}

} // namespace Engine::Renderer

#endif //SIMULATION_UTILS_HPP
