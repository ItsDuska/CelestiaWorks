#pragma once
#include "backend/vulkanAPI/core/CelestiaVulkanTypes.h"
#include "System/Vector.h"
#include <unordered_map>
#include <memory>

namespace celestia
{
	struct Character
	{
		Vec2i size;
		Vec2i bearing;
		uint32_t offset;
		uint32_t advance;
	};

	struct Font_t
	{
		RawTexture texture;
		std::unordered_map<char,const Character> characters;
		float invBmpWidth;
		uint32_t bmpHeigth;
	};


	class FontReader
	{
	public:
		static std::unique_ptr<Font_t> createFont(const char* filepath, uint8_t fontSize);
	};

}
