#pragma once
#include <vulkan/vulkan.h>
#include <celestiaTypes/CelestiaTypes.h>
#include <vector>


namespace celestia
{
	struct AllocatedBuffer
	{
		VkBuffer buffer;
		VkDeviceMemory memory;
	};

	struct RawMesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};

	struct Mesh
	{
		uint32_t vertexBufferSize;
		uint32_t indexBufferSize;
		AllocatedBuffer vertexBuffer;
		AllocatedBuffer indexBuffer;
	};
	
	struct Material
	{
		VkPipeline pipeline;
		VkPipelineLayout layout;
	};

	struct AllocatedImage
	{
		VkImage image;
		VkDeviceMemory memory;
	};

}