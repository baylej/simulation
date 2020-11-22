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
#include <glm/vec2.hpp>

using glm::vec2, glm::vec3;

namespace Engine::Contexts {

Scene2D::Scene2D(const Renderer::Renderer& renderer):
		renderer(renderer), camera(Main::get()->display_height, Main::get()->display_width), blitter(renderer),
		texture(Renderer::Image("data/debug_tex.png"))
		//texture(Renderer::bw_checker)
{
	renderer.use_program();
	renderer.set_proj_matrix(camera.get_proj_matrix());
	renderer.set_view_matrix(camera.get_view_matrix());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
}

void Scene2D::loop_run([[maybe_unused]] float delta_t)
{
	anim_t += delta_t;
	blitter.blit(texture, vec2((1280 - texture.get_width())/2., (720 - texture.get_height())/2.), -anim_t * 3.141592/10.);
	blitter.blit(texture, vec2(1280/2., 720/2. - texture.get_height()), glm::vec4(1., .5, .5, 1.));
	blitter.blit(texture, vec2(0), vec2(texture.get_width(), texture.get_height()), vec2(1280/4., 3*720/4.), vec2(200., 200.), {50, 50}, anim_t * 3.141592/20.);
	blitter.blit(texture, vec2(0), vec2(texture.get_width()*2, texture.get_height()), vec2(0), vec2(1280/2., 720/2.));
	blitter.blit(texture, vec2(0), vec2(texture.get_width(), texture.get_height()), vec2(1280/2., 720/2.), vec2(1280/2., 720/2.));

	blitter.rect_filled({0, 0}, {180, 108}, {0, 0, 0, .3});
	blitter.rect_filled({0, 0}, {50, 100}, {0, 0, 1, .5}, 3.141592/6.f);
	blitter.rect_filled({50, 0}, {50, 100}, {1, 1, 1, .5}, 3.141592/5.f);
	blitter.rect_filled({100, 0}, {50, 100}, {1, 0, 0, .5}, 3.141592/4.f);

	blitter.stream_vertices({0, 300,  10, 400,  100, 400}, GL_TRIANGLES, {1., 0., 0., 1.});
	blitter.polyline({400, 400,   500, 400,   500, 500,   600, 500,   600, 600}, 5.f, {0., 1., 0., 1.});
	blitter.polygon({400, 200,   500, 200,   550, 300,   450, 300}, 8.f, {0., 0., 1., 1.});
	blitter.circle({800, 600}, 5, 3.f);
	Renderer::check_gl_error("Scene2D::loop_run: after blitter.stream_vertices");

	ImGui::SetNextWindowPos({0, 0}); // Top-Left corner
	ImGui::Begin("Scene 2D Options", nullptr, 0);

	ImGui::Text("render time: %.3fs (%i FPS)", delta_t, static_cast<int>(1./delta_t));
	ImGui::Text("animation frame: %.3fs", anim_t);

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
