#include "Graphics/Text.h"
#include "backend/vulkanAPI/resources/FontReader.h"

celestia::Text::Text(const Vec2 position, const char* symbols, const Color color, Font* font)
	: position(position),color(color),font(font),dirty(true)
{
	this->symbols = symbols;
	static int textID = 1;
	id = textID;
	id++;
	updatePosition();
}

celestia::Font* celestia::Text::getFont()
{
	return font;
}

void celestia::Text::updatePosition()
{
	uint32_t indexoffset = 0;
	size_t len = symbols.size();
	vertices.resize(len*4);
	size = getTextSize();

	float x = 0.f;
	float scale = 1.f;

	for (size_t i = 0; i < len; i++)
	{
		const char c = symbols[i];
		const Character& character = font->bitmapData->characters[c];
		const float xpos = x + character.bearing.x * scale;
		const float ypos = (size.y - character.bearing.y) * scale;

		float w = static_cast<float>(character.size.x * scale);
		float hraw = static_cast<float>(character.size.y);
		float h = hraw * scale;
		float u0 = static_cast<float>(character.offset) * font->bitmapData->invBmpWidth;
		float v = (hraw) / font->bitmapData->bmpHeigth;
		float u1 = static_cast<float>((character.offset + character.size.x)) * font->bitmapData->invBmpWidth;

		const int offset = i * 4;

		vertices[offset] = { {xpos,ypos},{u0,0.f},{color.r,color.g,color.b},0};
		vertices[offset+1] = { { xpos + w,ypos},{u1,0.f},{color.r,color.g,color.b},0 };
		vertices[offset +2] = { {xpos + w,ypos + h},{u1,v},{color.r,color.g,color.b},0 };
		vertices[offset + 3] = { {xpos,ypos + h},{u0,v},{color.r,color.g,color.b},0 };
		
		x += (character.advance >> 6) * scale;

	}
}

celestia::Vec2 celestia::Text::getTextSize()
{
	Vec2 size;
	for (size_t i = 0; i < symbols.size(); i++) {
		char c = symbols[i];
		const Character& character = font->bitmapData->characters[c];
		float w = (float)character.size.x;
		float h = (float)character.size.y;

		size.x += (character.advance >> 6);
		size.y = std::max(size.y, h);
	}
	
	return size;
}

void celestia::Text::disableDirtyFlag()
{
	dirty = false;
}

