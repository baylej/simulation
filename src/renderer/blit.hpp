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
	explicit Blitter(const Renderer &renderer);
	~Blitter() = default;
	Blitter(const Blitter&) = delete;
	Blitter(Blitter&&) = delete;
	Blitter& operator=(const Blitter&) = delete;
	Blitter& operator=(Blitter&&) = delete;

	// Base blit function, used by more user friendly functions below
	void blit(const Texture& tex, const glm::mat4& model_matrix, const glm::mat3& texture_matrix, const glm::vec4& tint = glm::vec4(1.)) const;

	// Draw texture at the given coordinate
	void blit(const Texture& tex, glm::vec2 dst_pos, glm::vec4 tint = glm::vec4(1.)) const;

	// Draw texture rotated on its center
	void blit(const Texture& tex, glm::vec2 dst_pos, float angle, glm::vec4 tint = glm::vec4(1.)) const;

	// Use screen coordinates in pixels absolute (see Camera2D)
	void blit(const Texture& tex,
	          glm::vec2 src_pos, glm::vec2 src_dim,
	          glm::vec2 dst_pos, glm::vec2 dst_dim,
	          glm::vec2 center = {0,0}, float angle = 0.f, glm::vec4 tint = glm::vec4(1.)) const;

private:
	const Renderer& renderer;
	const struct Plane : public Static_indexed_mesh {
		Plane();
		~Plane() = default;
	} plane;
};

} // namespace Engine::Renderer

#endif //SIMULATION_BLIT_HPP
