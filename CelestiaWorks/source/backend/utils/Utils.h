#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "System/CelestiaTypes.h"


struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;

namespace celestia
{
	namespace utils
	{
		const std::vector<char> readFile(const char* filename);

		VkVertexInputBindingDescription getBindingDescription();

		std::array<VkVertexInputAttributeDescription, 4> getAttributeDescription();
		
		Vec4 normalizeColor(Color& color);
	}

}
