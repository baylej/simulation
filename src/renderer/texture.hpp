/*
    3D Physics Simulations - Texture: Image loading and OpenGL texture handling
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

#ifndef SIMULATION_TEXTURE_HPP
#define SIMULATION_TEXTURE_HPP

#include "utils.hpp"

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
	// Create a Texture from an Image (Copies the image into the VRAM, then the client side image can safely be destructed)
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
	[[nodiscard]] int get_width() const { return width; }
	[[nodiscard]] int get_height() const { return height; }

private:
	int width, height;
	GLuint texture_name;
};

} // namespace Engine::Renderer

#endif //SIMULATION_TEXTURE_HPP
