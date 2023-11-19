#pragma once
#include "vulkan/Device.h"

namespace celestia {

	class Buffer
	{
	public:
		Buffer(Device& device);
		~Buffer();

		Mesh* get_mesh(const std::string& name);
		
		void createBuffer(VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);

		void createMesh();
		void createVertexBuffer(Mesh& mesh);
		void createIndexBuffer(Mesh& mesh);
	private:
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		Device& device;

		std::unordered_map<std::string, Mesh> meshes;

	};

}
