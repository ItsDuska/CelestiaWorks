#pragma once
#include "Backend/VulkanAPI/Config/VulkanConfig.hpp"
#include <vulkan/vulkan.h>
#include "System/CelestiaTypes.hpp"
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
		std::vector<VertexBatch> vertices;
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

	struct FullDescriptorSet
	{
		VkDescriptorSet descriptor;
		VkDescriptorSetLayout layout;
	};

	struct DrawInfo
	{
		Material material;
		VkDescriptorSet descriptor;
		VkDescriptorSetLayout layout;
		Mesh* mesh;
		int amountToDraw;
	};

	struct UniformBufferObject
	{
		alignas(16) Mat4 transform;
	};

	struct PUSH_CONSTANTS // TODO: To something to this weirdness...
	{
		alignas(16) Mat4 projection;
	};

	struct Vec2Aligned
	{
		Vec2 vec;
		float padding[2]; // To align to 16 bytes
	};

} // namespace celestia