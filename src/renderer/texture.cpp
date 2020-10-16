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

#include <cassert>
#include <stb_image.h>

namespace Engine::Renderer {

Image::Image(const std::string& filename):
		width(0), height(0), channels(0), raster(nullptr)
{
	raster = stbi_load(filename.data(), &width, &height, &channels, 0);
	if (raster == nullptr) {
		throw std::runtime_error("Could not stbi_load file " + filename);
	}
}

Image::Image(int width, int height, int channels, unsigned char* raster):
		width(width), height(height), channels(channels), raster(raster)
{
	assert(raster != nullptr);
}

Image::~Image()
{
	if (raster != nullptr) {
		stbi_image_free(static_cast<void*>(raster));
	}
}

Image& Image::operator=(Image&& other) noexcept
{
	if (raster != nullptr && raster != other.raster) {
		stbi_image_free(static_cast<void*>(raster));
	}
	width = other.width;
	height = other.height;
	channels = other.channels;
	raster = other.raster;
	other.raster = nullptr;
	return *this;
}

Texture::Texture(const Image& image):
		width(image.get_width()), height(image.get_height())
{
	glGenTextures(1, &texture_name);
	glBindTexture(GL_TEXTURE_2D, texture_name);
	// FIXME set Min and Mag filters, Wrap settings, etc...
	// see: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
	GLenum format = GL_NONE;
	switch (image.get_channels()) {
		case 1: format = GL_LUMINANCE; break;
		case 2: format = GL_LUMINANCE_ALPHA; break;
		case 3: format = GL_RGB; break;
		default: format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.get_width(), image.get_height(), 0, format, GL_UNSIGNED_BYTE, image.get_raster());
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (texture_name != GL_NONE && texture_name != other.texture_name) {
		glDeleteTextures(1, &texture_name);
	}
	texture_name = other.texture_name;
	other.texture_name = GL_NONE;
	width = other.width;
	height = other.height;
	return *this;
}

Texture::~Texture()
{
	if (texture_name != GL_NONE) {
		glDeleteTextures(1, &texture_name);
	}
}

}
