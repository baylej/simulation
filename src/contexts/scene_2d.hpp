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
#pragma once

#ifndef SIMULATION_SCENE_2D_HPP
#define SIMULATION_SCENE_2D_HPP

#include "../context.hpp"
#include "../renderer/camera.hpp"
#include "../renderer/renderer.hpp"
#include "../renderer/blit.hpp"
#include "../renderer/texture.hpp"

namespace Engine::Contexts {

class Scene2D: public Context {
public:
	Scene2D(const Renderer::Renderer& renderer);

	void loop_run(float delta_t) override;
	void start() override;
private:
	const Renderer::Renderer& renderer;
	Renderer::Camera2D camera;
	Renderer::Blitter blitter;
	Renderer::Texture texture;
};

} // namespace Engine::Contexts

#endif //SIMULATION_SCENE_2D_HPP
