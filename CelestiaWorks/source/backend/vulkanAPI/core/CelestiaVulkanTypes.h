#pragma once
#include "backend/vulkanAPI/config/VulkanConfig.h"
#include <vulkan/vulkan.h>
#include "System/CelestiaTypes.h"
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

	struct RawTexture
	{
		AllocatedImage allocatedImage;
		VkImageView imageView;
		uint32_t textureID;
	};

	struct DrawInfo
	{
		Material *material;
		VkDescriptorSet* descriptors;
		VkDescriptorSetLayout layout;
		Mesh mesh;
		int amountToDraw;
	};


	struct UniformBufferObject
	{
		alignas(16) Mat4 transform;
	};

	struct PUSH_CONSTANTS //TODO: To something to this weirdness...
	{
		alignas(16) Mat4 projection;
	};

}