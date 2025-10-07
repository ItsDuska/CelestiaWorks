#include "Buffer.hpp"
#include "CommandBuffer.hpp"
#include "Backend/VulkanAPI/Core/Device.hpp"
#include "Backend/VulkanAPI/Config/VulkanConfig.hpp"

celestia::AllocatedBuffer
celestia::buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	AllocatedBuffer buffer{};

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if(vkCreateBuffer(Device::context.device, &bufferInfo, nullptr, &buffer.buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create buffer!");
	}

	VkMemoryRequirements memRequirments;
	vkGetBufferMemoryRequirements(Device::context.device, buffer.buffer, &memRequirments);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirments.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirments.memoryTypeBits, properties);

	if(vkAllocateMemory(Device::context.device, &allocInfo, nullptr, &buffer.memory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory");
	}

	vkBindBufferMemory(Device::context.device, buffer.buffer, buffer.memory, 0);

	return buffer;
}

/* TODO:
 * Tee ehk� kuston Vertex tai mesh class, jonka avulla voimme helpommin m��ritell� jokaisen data tyypin koon
 * tiedetyill� funktioilla kuten "this->getDataSize();"
 */

celestia::Mesh* celestia::buffer::createMesh(RawMesh& rawMesh)
{
	Mesh* mesh = new Mesh;
	mesh->indexBufferSize = static_cast<uint32_t>(rawMesh.indices.size() * sizeof(uint16_t));
	mesh->indexBuffer = createIndexBuffer(rawMesh.indices.data(), mesh->indexBufferSize);
	mesh->vertexBufferSize = static_cast<uint32_t>(rawMesh.vertices.size() * sizeof(VertexBatch));
	mesh->vertexBuffer = createVertexBuffer(rawMesh.vertices.data(), mesh->vertexBufferSize);
	return mesh;
}

void celestia::buffer::updateBuffer(
  AllocatedBuffer& dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* vertexData)
{
	AllocatedBuffer stagingBuffer = createBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(Device::context.device, stagingBuffer.memory, 0, dataSize, 0, &data);
	memcpy(data, vertexData, (size_t)dataSize);
	vkUnmapMemory(Device::context.device, stagingBuffer.memory);

	copyBuffer(stagingBuffer.buffer, dstBuffer.buffer, dataSize, 0, dstOffset);

	vkDestroyBuffer(Device::context.device, stagingBuffer.buffer, nullptr);
	vkFreeMemory(Device::context.device, stagingBuffer.memory, nullptr);
}

uint32_t celestia::buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(Device::context.physicalDevice, &memProperties);

	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

celestia::AllocatedBuffer
celestia::buffer::createVertexBuffer(void* vertices, size_t bufferSize, VkBufferUsageFlags flag)
{
	AllocatedBuffer stagingBuffer = createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(Device::context.device, stagingBuffer.memory, 0, bufferSize, 0, &data);
	memcpy(data, vertices, (size_t)bufferSize);
	vkUnmapMemory(Device::context.device, stagingBuffer.memory);

	AllocatedBuffer vertexBuffer =
	  createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, flag);

	copyBuffer(stagingBuffer.buffer, vertexBuffer.buffer, bufferSize, 0, 0);

	vkDestroyBuffer(Device::context.device, stagingBuffer.buffer, nullptr);
	vkFreeMemory(Device::context.device, stagingBuffer.memory, nullptr);

	Device::context.deletionQueue.pushFunction(
	  [=]()
	  {
		  vkDestroyBuffer(Device::context.device, vertexBuffer.buffer, nullptr);
		  vkFreeMemory(Device::context.device, vertexBuffer.memory, nullptr);
	  });

	return vertexBuffer;
}

celestia::AllocatedBuffer celestia::buffer::createIndexBuffer(void* indicies, size_t bufferSize)
{
	AllocatedBuffer stagingBuffer = buffer::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(Device::context.device, stagingBuffer.memory, 0, bufferSize, 0, &data);
	memcpy(data, indicies, (size_t)bufferSize);
	vkUnmapMemory(Device::context.device, stagingBuffer.memory);

	AllocatedBuffer indexBuffer = createBuffer(bufferSize,
	  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	copyBuffer(stagingBuffer.buffer, indexBuffer.buffer, bufferSize, 0, 0);

	vkDestroyBuffer(Device::context.device, stagingBuffer.buffer, nullptr);
	vkFreeMemory(Device::context.device, stagingBuffer.memory, nullptr);

	Device::context.deletionQueue.pushFunction(
	  [=]()
	  {
		  vkDestroyBuffer(Device::context.device, indexBuffer.buffer, nullptr);
		  vkFreeMemory(Device::context.device, indexBuffer.memory, nullptr);
	  });

	return indexBuffer;
}

void celestia::buffer::copyBuffer(
  VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(Device::context.commandPool, Device::context.device);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(
	  Device::context.graphicsQueue, Device::context.commandPool, Device::context.device, commandBuffer);
}
