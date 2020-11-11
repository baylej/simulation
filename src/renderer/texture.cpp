/*
    3D Physics Simulations - Texture
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
#include "texture.hpp"

#include <stb_image.h>

namespace Engine::Renderer {

Image::Image(const std::string& filename):
		width(0), height(0), channels(0), raster(nullptr), row_align(1), deleter(reinterpret_cast<void (*)(unsigned char*)>(stbi_image_free))
{
	raster = stbi_load(filename.data(), &width, &height, &channels, 0);
	if (raster == nullptr) {
		throw std::runtime_error("Image::ctor: Could not stbi_load file " + filename);
	}
}

Image::Image(int width, int height, int channels, unsigned char* raster, int row_align, void (*del)(unsigned char*)):
		width(width), height(height), channels(channels), raster(raster), row_align(row_align), deleter(del)
{
	if (row_align != 1 &&
	    row_align != 2 &&
	    row_align != 4 &&
	    row_align != 8) {
		throw std::runtime_error("Image::ctor: invalid row alignment value");
	}
}

Image::~Image()
{
	if (deleter != nullptr && raster != nullptr) {
		deleter(raster);
	}
}

Image& Image::operator=(Image&& other) noexcept
{
	if (deleter != nullptr && raster != nullptr && raster != other.raster) {
		deleter(raster);
	}
	width = other.width;
	height = other.height;
	channels = other.channels;
	raster = other.raster;
	other.raster = nullptr;
	return *this;
}

void Image::init()
{
	stbi_set_flip_vertically_on_load(true);
}

static const unsigned char colours[4] = { 0, 0xFF, 0xFF, 0 };
const Image bw_checker(2, 2, 1, const_cast<unsigned char*>(colours));

Texture::Texture(const Image& image, GLenum filtering, GLenum wrapping):
		dimensions(image.get_width(), image.get_height())
{
	if (image.get_raster() == nullptr) {
		throw std::runtime_error("Texture::ctor: Cannot create from empty Image");
	}
	glGenTextures(1, &texture_name);
	glBindTexture(GL_TEXTURE_2D, texture_name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

	glPixelStorei(GL_UNPACK_ALIGNMENT, image.get_row_align());

	GLenum format = GL_NONE;
	GLenum internal_format = GL_NONE;
	switch (image.get_channels()) {
		case 1: internal_format = GL_R8; format = GL_RED; break; // GL_LUMINANCE deprecated
		case 2: internal_format = GL_RG8; format = GL_RG; break; // GL_LUMINANCE_ALPHA deprecated
		case 3: internal_format = GL_RGB8; format = GL_RGB; break;
		default: internal_format = GL_RGBA8; format = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image.get_width(), image.get_height(), 0, format, GL_UNSIGNED_BYTE, image.get_raster());
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (texture_name != GL_NONE && texture_name != other.texture_name) {
		glDeleteTextures(1, &texture_name);
	}
	texture_name = other.texture_name;
	other.texture_name = GL_NONE;
	dimensions = other.dimensions;
	return *this;
}

Texture::~Texture()
{
	if (texture_name != GL_NONE) {
		glDeleteTextures(1, &texture_name);
	}
}

}
