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
#pragma once

#ifndef SIMULATION_SCENE_3D_HPP
#define SIMULATION_SCENE_3D_HPP

#include "../context.hpp"
#include "../renderer/camera.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/mesh.hpp"

#include <memory>

namespace Engine::Contexts {

class Scene3D : public Context {
public:
	Scene3D(const Renderer::Renderer& renderer, const Renderer::Static_indexed_mesh& mesh);
	~Scene3D() override = default;
	Scene3D(const Scene3D&) = delete;
	Scene3D(Scene3D&&) = delete;
	Scene3D& operator=(const Scene3D&) = delete;
	Scene3D& operator=(Scene3D&&) = delete;

	void start() final;
	void loop_run(float delta_t) final;
private:
	const Renderer::Renderer& renderer;
	const Renderer::Static_indexed_mesh& mesh;

	Renderer::Camera3D camera;
	glm::mat4 model_m4;

	float anim_t = 0;

	// GUI elements
	static constexpr const char* prim_type_names[] = { "GL_TRIANGLES", "GL_LINE_LOOP", "GL_POINTS" };
	static constexpr const GLenum prim_type_values[] = { GL_TRIANGLES, GL_LINE_LOOP, GL_POINTS };
	int prim_type_selected = 0;
};

}

#endif //SIMULATION_SCENE_3D_HPP
