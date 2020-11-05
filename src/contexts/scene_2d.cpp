/*
    3D Physics Simulations - Scene2D
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
#include "scene_2d.hpp"
#include "../main.hpp"

#include <imgui/imgui.h>

namespace Engine::Contexts {

Scene2D::Scene2D(const Renderer::Renderer& renderer):
		renderer(renderer), camera(Main::get()->display_height, Main::get()->display_width), blitter(renderer, Renderer::Plane::get()),
		texture(Renderer::Image("data/debug_tex.png"))
		//texture(Renderer::bw_checker)
{
	renderer.use_program();
	renderer.set_proj_matrix(camera.get_proj_matrix());
	renderer.set_view_matrix(camera.get_view_matrix());
}

void Scene2D::loop_run([[maybe_unused]] float delta_t)
{
	blitter.blit(texture,0, 0, texture.get_width()*2, texture.get_height(), 0, 0, 1280/2., 720/2.);
	blitter.blit(texture,0, 0, texture.get_width(), texture.get_height(), 1280/2., 720/2., 1280/2., 720/2.);

	ImGui::SetNextWindowPos({0, 0}); // Top-Left corner
	ImGui::Begin("Scene 2D Options", nullptr, 0);

	if (ImGui::Button("Back to Menu")) {
		Context_holder::get().set_context(Context_holder::get().menu);
	}

	ImGui::End();
}

void Scene2D::start()
{
	Context::start();
}

} // namespace Engine::Contexts
