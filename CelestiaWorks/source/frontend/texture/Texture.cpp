#include "Texture.h"
#include "backend/vulkanAPI/Image.h"

celestia::Texture::Texture()
	: size(0, 0), pixels(nullptr)
{
}

celestia::Texture::~Texture()
{
	Image::deleteTextureImage(*pixels);
}

bool celestia::Texture::loadTexture(const char* filepath)
{
	pixels = std::make_unique<RawTexture>(Image::createTextureImage(filepath, size));
	return pixels->imageView != nullptr;
}

celestia::Vec2i celestia::Texture::getSize() const
{
	return size;
}
