/*
    3D Physics Simulations - Main: create an OpenGL ES 3.0 context using GLFW
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
#include "main.hpp"
#include "context.hpp"
#include "menu.hpp"

#include <iostream>
using std::cout, std::endl, std::cerr;

// GLFW is the modern, portable way to create a Window and an OpenGL (ES) context
#include <GLFW/glfw3.h>

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

Context_holder Context_holder::instance{};
Context_holder& Context_holder::get() { return Context_holder::instance; };

void error_callback(int error, const char* description)
{
	cerr << "GLFW: Error " << error << ": " << description << endl;
}

static GLFWwindow* window = nullptr;

void main_loop()
{
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// ---

	// Render here
	Context_holder::get().get_context()->loop_run(0.f);

	// ---
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glFlush();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		cerr << "GLFW: cannot initialise" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES); // Multisampling (MSAA)
	if constexpr (!ALLOW_RESIZING) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	if constexpr (WANT_DEBUG_CTX) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (!static_cast<bool>(window)) {
		cerr << "GLFW: cannot create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	Context_holder::get().display_height = WINDOW_HEIGHT;
	Context_holder::get().display_width = WINDOW_WIDTH;

	Menu menu;
	Context_holder::get().menu = &menu;
	Context_holder::get().set_context(&menu);

	// DearImGui sets and resets the Viewport dimensions at ImGui_ImplOpenGL3_RenderDrawData
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// TODO install user defined input callbacks here before initialising DearImGui !!
	//glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int modifiers) { });
	//glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int modifiers) {});
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		Context_holder::get().display_height = h;
		Context_holder::get().display_width = w;
	});

	// ImGui init: create a dear imgui context, setup some options, load fonts
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	// TODO: Set optional io.ConfigFlags values.
	//   e.g. 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard' to enable keyboard controls.
	// TODO: Fill optional fields of the io structure later.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Main Loop
#ifdef EMSCRIPTEN
	emscripten_set_main_loop(main_loop, 0, 1);
#else
	while (!glfwWindowShouldClose(window)) {
		main_loop();
	}
#endif

	// Cleanup, shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void terminate()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}
