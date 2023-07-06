#include "VulkanTypes.h"


VkVertexInputBindingDescription celestia::Vertex::getBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> celestia::Vertex::getAttributeDescription()
{
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
	//position
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);
	//normal
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, normal);
	//color
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, color);
	return attributeDescriptions;
}

void celestia::DeletionQueue::pushFunction(std::function<void()>&& function)
{
	deletors.push_back(function);
}

void celestia::DeletionQueue::flush()
{
	for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
	{
		(*it)();
	}

	deletors.clear();
}
