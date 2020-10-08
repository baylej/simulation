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
#include "../3D/renderer.hpp"
#include "../3D/mesh.hpp"

#include <memory>

namespace Engine::Contexts {

class Scene3D : public Context {
public:
	Scene3D(N3D::Renderer& renderer, std::unique_ptr<N3D::Mesh> mesh);
	~Scene3D() final = default;

	void start() final;
	void loop_run(float delta_t) final;
private:
	N3D::Renderer& renderer;
	N3D::Camera camera;
	std::unique_ptr<N3D::Mesh> mesh;
};

}

#endif //SIMULATION_SCENE_3D_HPP
