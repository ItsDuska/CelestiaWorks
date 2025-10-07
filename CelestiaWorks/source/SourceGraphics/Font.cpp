#include "Graphics/Font.hpp"
#include "Backend/VulkanAPI/Resources/FontReader.hpp"
#include "Backend/VulkanAPI/Core/Image.hpp"

celestia::Font::Font() : bitmapData(nullptr)
{
}

celestia::Font::~Font()
{
	if(bitmapData == nullptr)
	{
		return;
	}
	Image::deleteTextureImage(bitmapData->texture);
}

bool celestia::Font::loadFont(const char* filepath, uint8_t fontSize)
{
	bitmapData = FontReader::createFont(filepath, fontSize);
	return bitmapData != nullptr;
}
