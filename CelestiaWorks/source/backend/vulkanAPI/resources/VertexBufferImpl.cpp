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
	mesh.vertexBuffer.buffer = nullptr;
	mesh.vertexBuffer.memory = nullptr;
	mesh.indexBuffer.buffer = nullptr;
	mesh.indexBuffer.memory = nullptr;
}

void celestia::VertexBufferImpl::create(Vertex* vertices, size_t vertexCount, DrawType drawType, Usage usage)
{
	mesh.vertexBufferSize = vertexCount * sizeof(Vertex);
	this->mesh.vertexBuffer = buffer::createVertexBuffer(vertices, mesh.vertexBufferSize, CelestiaUsageToVk(usage));

	if (usage == Usage::STATIC_INDEXED || usage == Usage::STREAM_INDEXED)
	{
		// WE HAVE ARE USING INDEX BUFFERS.
		std::vector<uint16_t> indicies = createIndexBufferForDrawType(vertexCount, drawType);

		mesh.indexBufferSize = indicies.size() * sizeof(uint16_t);
		this->mesh.indexBuffer = buffer::createIndexBuffer(indicies.data(), mesh.indexBufferSize);
	}
}


void celestia::VertexBufferImpl::freeBuffers() const
{
	if (mesh.vertexBuffer.buffer == nullptr)
	{
		std::cerr << "Error: tried to free buffer that was null.\n";
		return;
	}

	vkDestroyBuffer(Device::context.device, mesh.vertexBuffer.buffer, nullptr);
	if (mesh.indexBuffer.buffer != nullptr)
	{
		vkDestroyBuffer(Device::context.device, mesh.indexBuffer.buffer, nullptr);
	}
}

celestia::Mesh* celestia::VertexBufferImpl::getBufferPairPtr()
{
	return &mesh;
}
