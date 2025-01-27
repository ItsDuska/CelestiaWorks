#include "VertexBufferImpl.h"
#include "backend/vulkanAPI/core/Device.h"
#include "backend/vulkanAPI/core/Buffer.h"
#include "backend/core/IndexBufferGenerator.h"


static VkBufferUsageFlags CelestiaUsageToVk(celestia::Usage usage)
{
	switch (usage)
	{
	case celestia::Usage::STATIC:
		return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	case celestia::Usage::STATIC_INDEXED:
		return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	case celestia::Usage::STREAM:
		return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	case celestia::Usage::STREAM_INDEXED:
		return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	default:
		throw std::invalid_argument("Unsupported Usage");
	}
}


celestia::VertexBufferImpl::VertexBufferImpl()
{
	// a bit goofy xd.
	vertexBuffer.buffer = nullptr;
	vertexBuffer.memory = nullptr;
	indexBuffer.buffer = nullptr;
	indexBuffer.memory = nullptr;
}

void celestia::VertexBufferImpl::create(Vertex* vertices, size_t vertexCount, DrawType drawType, Usage usage)
{
	const size_t vertexBufferMemorySize = vertexCount * sizeof(Vertex);
	this->vertexBuffer = buffer::createVertexBuffer(vertices, vertexBufferMemorySize,CelestiaUsageToVk(usage));

	if (usage == Usage::STATIC_INDEXED || usage == Usage::STREAM_INDEXED)
	{
		// WE HAVE ARE USING INDEX BUFFERS.
		std::vector<uint16_t> indicies = createIndexBufferForDrawType(vertexCount, drawType);

		const size_t indexBufferMemorySize = indicies.size() * sizeof(uint16_t);
		this->indexBuffer = buffer::createIndexBuffer(indicies.data(), indexBufferMemorySize);
	}
}


void celestia::VertexBufferImpl::freeBuffers() const
{
	vkDestroyBuffer(Device::context.device, vertexBuffer.buffer, nullptr);
	if (indexBuffer.buffer != nullptr)
	{
		vkDestroyBuffer(Device::context.device, indexBuffer.buffer, nullptr);
	}
}
