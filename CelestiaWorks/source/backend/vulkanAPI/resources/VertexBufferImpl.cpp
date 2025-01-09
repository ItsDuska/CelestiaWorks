#include "VertexBufferImpl.h"
#include "backend/vulkanAPI/core/Device.h"
#include "backend/vulkanAPI/core/Buffer.h"


void celestia::VertexBufferImpl::create(Vertex* vertices, size_t size)
{
	this->vertexBuffer = buffer::createVertexBuffer(vertices, size);

	const size_t bufferSize = 6 * (size / sizeof(Vertex));
	std::vector<uint16_t> indices(bufferSize);
	uint16_t offset = 0;

	for (int i = 0; i < bufferSize; i += 6)
	{
		indices[static_cast<size_t>(i + 0)] = offset;
		indices[static_cast<size_t>(i + 1)] = 1u + offset;
		indices[static_cast<size_t>(i + 2)] = 2u + offset;

		indices[static_cast<size_t>(i + 3)] = 2u + offset;
		indices[static_cast<size_t>(i + 4)] = 3u + offset;
		indices[static_cast<size_t>(i + 5)] = offset;

		offset += 4;
	}
	
	this->indexBuffer = buffer::createIndexBuffer(indices.data(), bufferSize * sizeof(uint16_t));
}

void celestia::VertexBufferImpl::setUsage(Usage usage)
{
	this->currentUsage = usage;
}

void celestia::VertexBufferImpl::setDrawType(DrawType type)
{
	this->type = type;
}

void celestia::VertexBufferImpl::freeBuffers()
{
	vkDestroyBuffer(Device::context.device, vertexBuffer.buffer, nullptr);
	vkDestroyBuffer(Device::context.device, indexBuffer.buffer, nullptr);
}
