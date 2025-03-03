#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "System/CelestiaTypes.h"


struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;
enum VkFormat;

namespace celestia
{
	namespace utils
	{
		const std::vector<uint32_t> readFileAsU32(const char* filename);

		//VkVertexInputBindingDescription getBindingDescription();

		std::array<VkVertexInputAttributeDescription, 4> getAttributeDescription();
		
		Vec4 normalizeColor(Color& color);

		// new way
		VkVertexInputBindingDescription createBindingDescription(uint32_t binding = 0, uint32_t stride = sizeof(Vertex), VkVertexInputRate inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX);


		class CustomVertexInputAttributeDescriptionFactory
		{
		public:
			CustomVertexInputAttributeDescriptionFactory() = default;
			void pushDescription(uint32_t binding, VkFormat format, uint32_t offset);

			int getSize();
			VkVertexInputAttributeDescription* rawData();

		private:
			std::vector<VkVertexInputAttributeDescription> descriptionList;
		};


	}

	

}
