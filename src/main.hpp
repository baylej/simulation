/*
    3D Physics Simulations - Main: includes GL ES3 and GLM and DearImGui and other headers
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

#ifndef SIMULATION_MAIN_HPP
#define SIMULATION_MAIN_HPP

// Math lib
// FIXME: set IM_VEC2_CLASS_EXTRA and IM_VEC4_CLASS_EXTRA to cast from glm::vec2 and glm::vec4 as explained in imconfig.h ?
#include <glm/glm.hpp>

// ImGui is included first in example programs
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// OpenGL ES 3, no loader needed unless you need a specific extension
#include <GLES3/gl3.h>

// Configuration & Defaults
constexpr bool WANT_DEBUG_CTX = true;
constexpr unsigned MSAA_SAMPLES = 2; // 0 to disable
constexpr int WINDOW_HEIGHT = 720;
constexpr int WINDOW_WIDTH = 1280;
constexpr const char* WINDOW_TITLE = "Simulation";
constexpr bool ALLOW_RESIZING = true;

void terminate();

#endif //SIMULATION_MAIN_HPP
