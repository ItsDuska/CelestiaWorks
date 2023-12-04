#pragma once
#include <vulkan/vulkan.h>
#include "CelestiaVulkanTypes.h"


namespace celestia
{
	class Device;


	class CELESTIA_WORKS Buffer
	{
	public:
		Buffer(Device& device);
		~Buffer();

		AllocatedBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void createMesh(RawMesh& rawMesh, Mesh& mesh);

		Mesh& getDefaultMesh();
		//template<typename DataType> void mapBuffer(const DataType *data, AllocatedBuffer& buffer, VkDeviceSize size);
	private:
		Device& device;
		Mesh defaultMesh;
	private:
		AllocatedBuffer createVertexBuffer(RawMesh& rawMesh);
		AllocatedBuffer createIndexBuffer(RawMesh& rawMesh);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}