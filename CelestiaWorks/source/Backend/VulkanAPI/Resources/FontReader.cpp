#include "FontReader.hpp"
#include "Backend/VulkanAPI/Core/Image.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

// #define CELESTIA_FULL_RELEASE

std::unique_ptr<celestia::Font_t> celestia::FontReader::createFont(const char* filepath, uint8_t fontSize)
{
	if(filepath == nullptr)
	{
		return nullptr;
	}

	if(fontSize < 1u || fontSize > 100u)
	{
		fontSize = 16u;
	}

	FT_Library ft;
	FT_Error error = FT_Init_FreeType(&ft);
	if(error)
	{
		std::cout << error << "\n";
		std::cerr << "ERROR: Unable to init freetype lib!\n";
		return nullptr;
	}

	FT_Face face;
	error = FT_New_Face(ft, filepath, 0, &face);

	if(error == FT_Err_Unknown_File_Format)
	{
		std::cerr
		  << "ERROR: The font file could be opened and read, but it appears that its font format is unsupported!\n";
		return nullptr;
	}
	else if(error)
	{
		std::cerr << "ERROR: the font file could not be opened or read! The filepath might be wrong or something else "
					 "is bonked...\n";
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	std::unique_ptr<Font_t> font = std::make_unique<Font_t>();

	// creating pixelBuffer for fontmap.

	uint32_t bmpWidth = 0;
	std::unordered_map<char, std::vector<uint8_t>> data;
	for(unsigned char letter = 0; letter < 128; letter++)
	{
		error = FT_Load_Char(face, letter, FT_LOAD_RENDER);
		if(error)
		{
			std::cerr << "ERROR: Failed to load char!\n";
			return nullptr;
		}

		font->bmpHeigth = std::max(bmpWidth, face->glyph->bitmap.rows);

		unsigned int pitch = face->glyph->bitmap.pitch;
		const Character character = {Vec2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		  Vec2i(face->glyph->bitmap_left, face->glyph->bitmap_top), bmpWidth,
		  static_cast<unsigned int>(face->glyph->advance.x)};

		font->characters.insert(std::pair<char, const Character>(letter, character));

		if(face->glyph->bitmap.width > 0)
		{
			void* bufferPtr = face->glyph->bitmap.buffer;

			const int rows = face->glyph->bitmap.rows;
			const int width = face->glyph->bitmap.width;

			std::vector<uint8_t> charData(width * rows);

			for(int i = 0; i < rows; i++)
			{
				for(int j = 0; j < width; j++)
				{
					uint8_t byte = face->glyph->bitmap.buffer[i * pitch + j];
					charData[static_cast<size_t>(i * pitch + j)] = byte;
				}
			}
			data.insert(std::pair<char, std::vector<uint8_t>>(letter, charData));
		}
		bmpWidth += face->glyph->bitmap.width;
	}

	error = FT_Done_Face(face);
	if(error)
	{
		std::cerr << "ERROR: FreeType Face failed!\n";
		return nullptr;
	}

	font->invBmpWidth = 1 / static_cast<float>(bmpWidth);

	std::vector<uint8_t> pixelBuffer(bmpWidth * font->bmpHeigth);

	uint32_t xpos = 0;
	for(unsigned char letter = 0; letter < 128; letter++)
	{
		const Character& character = font->characters[letter];
		std::vector<uint8_t>& charData = data[letter];
		uint32_t width = character.size.x;
		uint32_t height = character.size.y;

		for(uint32_t i = 0; i < height; i++)
		{
			for(uint32_t j = 0; j < width; j++)
			{
				uint8_t byte = charData[i * width + j];
				pixelBuffer[i * bmpWidth + xpos + j] = byte;
			}
		}
		xpos += width;
	}

	// craeting VkTexture

	// Validate bitmap dimensions before creating texture
	if(bmpWidth <= 0 || font->bmpHeigth <= 0)
	{
		std::cerr << "ERROR: Invalid font bitmap dimensions: " << bmpWidth << "x" << font->bmpHeigth << std::endl;
		FT_Done_FreeType(ft);
		return nullptr;
	}

	VkDeviceSize textureSize = static_cast<VkDeviceSize>(bmpWidth) *
							   font->bmpHeigth; // * pixelSize; // tätä ei tarvi koska pixelin koko on valmiiksi jo 1

	if(textureSize == 0)
	{
		std::cerr << "ERROR: Font texture size is zero!" << std::endl;
		FT_Done_FreeType(ft);
		return nullptr;
	}

	if(pixelBuffer.empty())
	{
		std::cerr << "ERROR: Pixel buffer is empty!" << std::endl;
		FT_Done_FreeType(ft);
		return nullptr;
	}

	Vec2i bitmapSize(bmpWidth, font->bmpHeigth);
	Image::createTextureFromBuffer(pixelBuffer.data(), textureSize, bitmapSize, font->texture, VK_FORMAT_R8_UNORM);

	FT_Done_FreeType(ft);

	return font;
}
