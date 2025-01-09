#include "Graphics/VertexBuffer.h"
#include "backend/vulkanAPI/resources/VertexBufferImpl.h"

// TODO: Tee tää loppuun... guuuuuh bwaaaaaaaaaaaaaaa
celestia::VertexBuffer::VertexBuffer(size_t size, Usage usage, DrawType type)
{
	this->size = size;
	this->bufferImpl = std::make_unique<VertexBufferImpl>(); // TODO: make vulkan backend of this.
}

celestia::VertexBuffer::VertexBuffer(Usage usage, DrawType type)
{
	size = 0;
	this->bufferImpl = std::make_unique<VertexBufferImpl>();
	bufferImpl->setUsage(usage);
	bufferImpl->setDrawType(type);
}

// TODO:
celestia::VertexBuffer::~VertexBuffer()
{
	// Destroy and free the buffer mem here.
	bufferImpl->freeBuffers();
}

void celestia::VertexBuffer::create(Vertex* vertices, size_t size)
{

}

// recreate the entire buffer with the new VK buffer flag.
void celestia::VertexBuffer::setUsage(Usage usage)
{
	bufferImpl->setUsage(usage);
}

void celestia::VertexBuffer::setDrawType(DrawType type)
{
	bufferImpl->setDrawType(type);
}



