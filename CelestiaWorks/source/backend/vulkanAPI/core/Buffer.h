#pragma once
#include <vulkan/vulkan.h>
#include "backend/vulkanAPI/core/CelestiaVulkanTypes.h"


namespace celestia
{
	namespace buffer
	{
		AllocatedBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		Mesh* createMesh(RawMesh& rawMesh);

		void updateBuffer(AllocatedBuffer& dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* vertexData);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		AllocatedBuffer createVertexBuffer(void* vertices, size_t bufferSize);
		AllocatedBuffer createIndexBuffer(void* data, size_t bufferSize);

		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);
	}

}