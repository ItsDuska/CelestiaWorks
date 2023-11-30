#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <celestiaTypes/CelestiaTypes.h>



namespace celestia
{
	struct AllocatedBuffer
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
		AllocatedBuffer vertexBuffer;
		AllocatedBuffer indexBuffer;
	};
	
}