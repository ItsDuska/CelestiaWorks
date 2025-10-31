#include "Graphics/Texture.hpp"
#include "Backend/VulkanAPI/Core/Image.hpp"

celestia::Texture::Texture() : size(0, 0), pixels(nullptr), nonOwningPixels(nullptr), ownsTexture(true)
{
}

celestia::Texture::Texture(const RawTexture* rawTexture, Vec2i textureSize, bool ownsTexture) 
	: size(textureSize), pixels(nullptr), nonOwningPixels(rawTexture), ownsTexture(ownsTexture)
{
}

celestia::Texture::~Texture()
{
	if (ownsTexture && pixels)
	{
		Image::deleteTextureImage(*pixels);
	}
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

const celestia::RawTexture* celestia::Texture::getRawTexturePtr() const
{
	return ownsTexture ? pixels.get() : nonOwningPixels;
}
