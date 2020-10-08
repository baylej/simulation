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
#include "../3D/utils.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace Engine::Contexts {

void Scene3D::start()
{
	Context::start();
}

void Scene3D::loop_run(float delta_t)
{
	renderer.set_proj_view_matrices(camera);
	renderer.set_model_matrix(glm::identity<glm::mat4>());
	mesh->draw();
#ifndef NDEBUG
	N3D::check_gl_error("Scene3D::loop_run");
#endif
}

Scene3D::Scene3D(N3D::Renderer& renderer, std::unique_ptr<N3D::Mesh> mm):
	renderer(renderer)
{
	mesh = std::move(mm);
	renderer.use_program();
}

}
