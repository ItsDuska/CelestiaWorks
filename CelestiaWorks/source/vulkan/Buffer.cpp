#include "Buffer.h"

celestia::Buffer::Buffer(Device& device)
	: device{ device }
{
}

celestia::Buffer::~Buffer()
{
	
}

celestia::Mesh* celestia::Buffer::get_mesh(const std::string& name)
{
	auto it = meshes.find(name);
	if (it == meshes.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}

void celestia::Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirments;
	vkGetBufferMemoryRequirements(device.getDevice(), buffer, &memRequirments);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirments.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirments.memoryTypeBits, properties);

	if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(device.getDevice(), buffer, bufferMemory, 0);
}

void celestia::Buffer::createMesh()
{
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f,0.f},{0.f,0.f,0.f}, {1.0f, 0.1f, 0.1f}},
		{{0.5f, -0.5f,0.f},{0.f,0.f,0.f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f,0.f},{0.f,0.f,0.f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f,0.f},{0.f,0.f,0.f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};


	Mesh mesh;

	mesh.indices = indices;
	mesh.vertices = vertices;

	createVertexBuffer(mesh);
	createIndexBuffer(mesh);

	meshes["temp"] = mesh;
}

void celestia::Buffer::createVertexBuffer(Mesh &mesh)
{
	VkDeviceSize bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, mesh.vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device.getDevice(), stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		mesh.vertexAllocation.buffer, mesh.vertexAllocation.bufferMemory);

	copyBuffer(stagingBuffer, mesh.vertexAllocation.buffer, bufferSize);

	vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);

	device.deletionQueue.pushFunction([=]() {
		vkDestroyBuffer(device.getDevice(), mesh.vertexAllocation.buffer, nullptr);
		vkFreeMemory(device.getDevice(), mesh.vertexAllocation.bufferMemory, nullptr);
		});
}

void celestia::Buffer::createIndexBuffer(Mesh& mesh)
{
	VkDeviceSize bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, mesh.indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device.getDevice(), stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		mesh.indiciesAllocation.buffer, mesh.indiciesAllocation.bufferMemory);

	copyBuffer(stagingBuffer, mesh.indiciesAllocation.buffer, bufferSize);

	vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
	device.deletionQueue.pushFunction([=]() {
		vkDestroyBuffer(device.getDevice(), mesh.indiciesAllocation.buffer, nullptr);
		vkFreeMemory(device.getDevice(), mesh.indiciesAllocation.bufferMemory, nullptr);
		});
}

void celestia::Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(device.getGraphicsQueue());
	vkFreeCommandBuffers(device.getDevice(), device.getCommandPool(), 1, &commandBuffer);
}

uint32_t celestia::Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevice(), &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}
