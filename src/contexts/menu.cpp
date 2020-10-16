/*
    3D Physics Simulations - Menu context
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
#include "menu.hpp"
#include "../main.hpp"
#include "scene_3d.hpp"
#include "scene_2d.hpp"

#include <imgui/imgui.h>

namespace Engine::Contexts {

void Menu::loop_run([[maybe_unused]] float delta_t)
{
	Main* main = Main::get();
	ImGui::SetNextWindowPos(ImVec2{main->display_width / 2.f, main->display_height / 2.f}, 0, ImVec2{.5f, .5f});
	ImGui::SetNextWindowSize(ImVec2{0.f, 0.f});
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration);

	if (ImGui::Button("Help")) {
		show_help = !show_help;
	}
	if (ImGui::Button("OpenGL triangle")) {
		new_ctx = std::make_unique<Scene3D>(*renderer, Renderer::Triangle::get());
		Context_holder::get().set_context(new_ctx.get());
	}
	if (ImGui::Button("OpenGL plane")) {
		new_ctx = std::make_unique<Scene2D>(*renderer);
		Context_holder::get().set_context(new_ctx.get());
	}
	/*if (ImGui::Button("Simulation 3")) {
		// TODO
	}
	if (ImGui::Button("Simulation 4")) {
		// TODO
	}*/

#ifndef EMSCRIPTEN
	if (ImGui::Button("Exit")) {
		Main::get()->terminate();
	}
#endif

	ImGui::End();

	if (show_help) {
		// TODO

	}
}

Menu::Menu()
{
	renderer = std::make_unique<Renderer::Renderer>();
}

} // namespace Engine::Contexts
