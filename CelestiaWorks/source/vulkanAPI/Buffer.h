#pragma once
#include <vulkan/vulkan.h>
#include "CelestiaVulkanTypes.h"


namespace celestia
{
	class Device;


	class Buffer
	{
	public:
		Buffer(Device& device);
		~Buffer();

		AllocatedBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void createMesh(RawMesh& rawMesh, Mesh& mesh);

		Mesh* getDefaultMesh();

		void updateBatchBuffer(AllocatedBuffer& dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const BatchVertex* srcdata);
		//template<typename DataType> void mapBuffer(const DataType *data, AllocatedBuffer& buffer, VkDeviceSize size);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		Device& device;
		Mesh defaultMesh;
	private:
		AllocatedBuffer createVertexBuffer(RawMesh& rawMesh);
		AllocatedBuffer createIndexBuffer(RawMesh& rawMesh);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);
	};

}