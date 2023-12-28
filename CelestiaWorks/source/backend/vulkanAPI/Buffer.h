#pragma once
#include <vulkan/vulkan.h>
#include "celestiaTypes/CelestiaVulkanTypes.h"


namespace celestia
{
	class Buffer
	{
	public:
		Buffer();
		~Buffer();

		static AllocatedBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void createMesh(RawMesh& rawMesh, Mesh& mesh);

		void updateBatchBuffer(AllocatedBuffer& dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const Vertex* srcdata);
		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	public:
		//TEMP
		std::vector<AllocatedBuffer> uniformBufferTemp;
		std::vector<void*> uniformBuffersMapped;
		//TEMP
	private:
		AllocatedBuffer createVertexBuffer(RawMesh& rawMesh);
		AllocatedBuffer createIndexBuffer(RawMesh& rawMesh);
		void createUniformBuffers();

		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);
	};

}