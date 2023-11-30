#pragma once
#include <iostream>
#include <vector>
#include <array>

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif



struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;

namespace celestia
{

	namespace utils
	{
		const std::vector<char> readFile(const char* filename);

		VkVertexInputBindingDescription getBindingDescription();

		std::array<VkVertexInputAttributeDescription, 3> getAttributeDescription();
		

	}

}
