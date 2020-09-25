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
#include "main.hpp"

void Menu::loop_run(float delta_t)
{
	Context_holder& ctxhldr = Context_holder::get();
	ImGui::SetNextWindowPos(ImVec2{ctxhldr.display_width/2.f, ctxhldr.display_height/2.f}, 0, ImVec2{.5f, .5f});
	ImGui::SetNextWindowSize(ImVec2{0.f, 0.f});
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration);

	if (ImGui::Button("Help")) {
		show_help = !show_help;
	}
	if (ImGui::Button("Simulation 1")) {
		// TODO
	}
	if (ImGui::Button("Simulation 2")) {
		// TODO
	}
	if (ImGui::Button("Simulation 3")) {
		// TODO
	}
	if (ImGui::Button("Simulation 4")) {
		// TODO
	}

#ifndef EMSCRIPTEN
	if (ImGui::Button("Exit")) {
		terminate();
	}
#endif

	ImGui::End();

	if (show_help) {
		// TODO

	}
}

void Menu::start()
{

}