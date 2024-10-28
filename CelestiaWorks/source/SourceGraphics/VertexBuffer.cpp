#include "Graphics/VertexBuffer.h"
#include "backend/vulkanAPI/resources/VertexBufferImpl.h"

// TODO: Tee tää loppuun... guuuuuh bwaaaaaaaaaaaaaaa
celestia::VertexBuffer::VertexBuffer(size_t size, Usage usage)
{
	this->size = size;
	this->currentUsage = usage;
	this->indexBuffer = nullptr; // TODO: Finish this.
	this->bufferImpl = nullptr; // TODO: make vulkan backend of this.
}

celestia::VertexBuffer::VertexBuffer(Usage usage)
{
	currentUsage = usage;
	size = 0;
	indexBuffer = nullptr;
	bufferImpl = nullptr;
}

// TODO:
celestia::VertexBuffer::~VertexBuffer()
{
	// Destroy and free the buffer mem here.
}

void celestia::VertexBuffer::create(Vertex* vertices, size_t size)
{

}


// recreate the entire buffer with the new VK buffer flag.
void celestia::VertexBuffer::setUsage(Usage usage)
{
	this->currentUsage = usage;
}



