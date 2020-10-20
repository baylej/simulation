/*
    3D Physics Simulations - Blit: A texture blitting class
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

#ifndef SIMULATION_BLIT_HPP
#define SIMULATION_BLIT_HPP

#include "renderer.hpp"
#include "mesh.hpp"
#include "texture.hpp"

namespace Engine::Renderer {

// A texture blitter to use with the Camera2D
class Blitter {
public:
	Blitter(const Renderer &renderer, const Plane &plane);
	~Blitter() = default;
	Blitter(const Blitter&) = delete;
	Blitter(Blitter&&) = delete;
	Blitter& operator=(const Blitter&) = delete;
	Blitter& operator=(Blitter&&) = delete;

	// Use screen coordinates in pixels absolute (see Camera2D)
	void blit(const Texture& tex,
	       float src_x, float src_y, float src_w, float src_h,
	       float dst_x, float dst_y, float dst_w, float dst_h) const; // TODO: add rotation and blend colour

private:
	const Renderer& renderer;
	const Plane& plane;
};

} // namespace Engine::Renderer

#endif //SIMULATION_BLIT_HPP
