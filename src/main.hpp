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

// GLFW is the modern, portable way to create a Window and an OpenGL (ES) context
#include <GLFW/glfw3.h>

// Forward declaration of main function required to add it as a friend to class Main
int main();

namespace Engine {

// Configuration & Defaults
constexpr bool WANT_DEBUG_CTX = true;
constexpr unsigned MSAA_SAMPLES = 2; // 0 to disable
constexpr int WINDOW_HEIGHT = 720;
constexpr int WINDOW_WIDTH = 1280;
constexpr const char* WINDOW_TITLE = "Simulation";
constexpr bool ALLOW_RESIZING = true;

class Main {
public:
	void init(); // Call once at the very beginning
	void main_loop(); // you're probably looking for `run_main_loop()`
	void run_main_loop(); // Does not return, runs `main_loop()` in loop
	void cleanup(); // Call once at the very end

	void terminate(); // Call it to exit from `run_main_loop()` gracefully

	static Main* get() { return instance; };

	// Display width and height, updated by a callback
	int display_width = WINDOW_WIDTH;
	int display_height = WINDOW_HEIGHT;

private:
	GLFWwindow* window = nullptr;
	static Main* instance; // Please set the instance (in main())

	static void error_callback(int, const char*);

	friend int ::main(); // Main can only be constructed in function main()

	Main() = default;
	~Main() = default;
	Main(const Main&) = delete;
	Main(const Main&&) = delete;
	Main& operator=(const Main&) = delete;
	Main& operator=(Main&&) = delete;
};

} // namespace Engine

#endif //SIMULATION_MAIN_HPP
