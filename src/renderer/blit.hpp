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

#include <string>

namespace Engine::Renderer {

class Image {
public:
	// ctor
	Image(const std::string& filename);
	Image(int width, int height, int channels, unsigned char* raster);

	// move
	Image(Image&& other) noexcept { *this = std::move(other); };
	Image& operator=(Image&&) noexcept;

	// dtor
	~Image();

	// no copy
	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;

	[[nodiscard]] int get_width() const { return width; }
	[[nodiscard]] int get_height() const { return height; }
	[[nodiscard]] int get_channels() const { return channels; }
	[[nodiscard]] const unsigned char* get_raster() const { return raster; }

private:
	int width, height;
	int channels; // number of components (1=Gs 2=GsA 3=RGB 4=RGBA)
	unsigned char* raster; // managed resource
};

class Texture {
public:
	// Create a Texture from an Image (Copies the image into the VRAM, then client side image can safely be destructed after)
	Texture(const Image& image);

	// move
	Texture(Texture&& other) noexcept { *this = std::move(other); }
	Texture& operator=(Texture&&) noexcept;

	// no copy
	Texture(const Texture&) = delete;

	Texture& operator=(const Texture&) = delete;

	// dtor
	~Texture();

	[[nodiscard]] GLuint get_texture_name() const { return texture_name; }

private:
	GLuint texture_name;
};
/*
// A texture blitter to use with the Camera2D
class Blitter {
public:
	Blitter(Renderer &renderer, Plane &plane);
	~Blitter() = default;
	Blitter(const Blitter&) = delete;
	Blitter(Blitter&&) = delete;
	Blitter& operator=(const Blitter&) = delete;
	Blitter& operator=(Blitter&&) = delete;

	// Use screen coordinates in pixels absolute (see Camera2D)
	void blit(GLuint tex_name,
	       float src_x, float src_y, float src_w, float src_h,
	       float dst_x, float dst_y, float dst_w, float dst_h);

private:
	const Renderer& renderer;
	const Plane& plane;
};
*/
} // namespace Engine::Renderer

#endif //SIMULATION_BLIT_HPP
