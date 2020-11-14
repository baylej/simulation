/*
    3D Physics Simulations - Scene3d
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
#include "scene_3d.hpp"
#include "../renderer/utils.hpp"
#include "../main.hpp"

#include <imgui/imgui.h>

#include <glm/ext/matrix_transform.hpp>

namespace Engine::Contexts {

void Scene3D::start()
{
	renderer.set_tint_colour(glm::vec4(1.));
}

void Scene3D::loop_run([[maybe_unused]] float delta_t)
{
	anim_t += delta_t;
	camera.set_pos({0, 0, .5 + glm::abs(glm::cos(anim_t))});
	camera.update_camera();
	renderer.set_proj_matrix(camera.get_proj_matrix());
	renderer.set_view_matrix(camera.get_view_matrix());
	renderer.set_model_matrix(model_m4);
	renderer.set_has_texture(false);
	mesh.draw(prim_type_values[prim_type_selected]);
#ifndef NDEBUG
	Renderer::check_gl_error("Scene3D::loop_run");
#endif
	ImGui::SetNextWindowPos({0, 0}); // Top-Left corner
	ImGui::Begin("Scene 3D Options", nullptr, 0);

	ImGui::Combo("Primitive type", &prim_type_selected, prim_type_names, IM_ARRAYSIZE(prim_type_names), IM_ARRAYSIZE(prim_type_names));
	if (ImGui::Button("Back to Menu")) {
		Context_holder::get().set_context(Context_holder::get().menu);
	}

	ImGui::End();
}

Scene3D::Scene3D(const Renderer::Renderer& renderer, const Renderer::Static_indexed_mesh& mm):
	renderer(renderer),
	mesh(mm),
	camera()
{
	camera.set_perspective_projection();
	renderer.use_program();
	model_m4 = glm::mat4(1.);
}

}
