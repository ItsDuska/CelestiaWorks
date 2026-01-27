#include "Graphics/Font.hpp"
#include "Backend/VulkanAPI/Resources/FontReader.hpp"
#include "Vulkan/Image.hpp"
#include <iostream>
#include <vulkan/vulkan.h>

celestia::Font::Font() : bitmapData(nullptr)
{
}

celestia::Font::~Font()
{
	if(bitmapData == nullptr)
	{
		return;
	}
	// Only delete if the texture was successfully created
	if(bitmapData->texture.allocatedImage.image != VK_NULL_HANDLE)
	{
		vk::Image::deleteTextureImage(bitmapData->texture);
	}
}

bool celestia::Font::loadFont(const char* filepath, uint8_t fontSize)
{
	bitmapData = FontReader::createFont(filepath, fontSize);
	if(bitmapData == nullptr)
	{
		std::cerr << "ERROR: Failed to load font from " << filepath << std::endl;
		return false;
	}
	return true;
}
