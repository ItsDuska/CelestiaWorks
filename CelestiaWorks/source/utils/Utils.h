#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "celestiaTypes/CelestiaTypes.h"

struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;




namespace celestia
{
	namespace utils
	{
		const std::vector<char> readFile(const char* filename);

		VkVertexInputBindingDescription getBindingDescription();

		std::array<VkVertexInputAttributeDescription, 3> getAttributeDescription();
		
		Vec4 normalizeColor(Color& color);
	}

}
