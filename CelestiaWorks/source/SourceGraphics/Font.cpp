#include "Graphics/Font.h"
#include "backend/vulkanAPI/resources/FontReader.h"
#include "backend/vulkanAPI/core/Image.h"

celestia::Font::Font()
	: bitmapData(nullptr)
{
}

celestia::Font::~Font()
{
	if (bitmapData == nullptr)
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
