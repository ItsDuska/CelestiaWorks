#include "Graphics/VertexBuffer.h"
#include "backend/vulkanAPI/resources/VertexBufferImpl.h"
#include <stdexcept>

// TODO: Tee tää loppuun... guuuuuh bwaaaaaaaaaaaaaaa
celestia::VertexBuffer::VertexBuffer(size_t size, Usage usage, DrawType type)
{
	this->size = size;
	this->usage = usage;
	this->type = type;
	this->bufferImpl = std::make_unique<VertexBufferImpl>(); // TODO: make vulkan backend of this.
}

celestia::VertexBuffer::VertexBuffer(Usage usage, DrawType type)
{
	size = 0;
	this->bufferImpl = std::make_unique<VertexBufferImpl>();
	this->usage = usage;
	this->type = type;
}

// TODO:
celestia::VertexBuffer::~VertexBuffer()
{
	// Destroy and free the buffer mem here.
	bufferImpl->freeBuffers();
}

void celestia::VertexBuffer::create(Vertex* vertices, size_t size)
{
	if (vertices == nullptr || size == 0)
	{
		throw std::invalid_argument("Vertex buffers data is null or the size of the data is zero");
	}
	bufferImpl->create(vertices, size, type, usage);
}

// recreate the entire buffer with the new VK buffer flag.
void celestia::VertexBuffer::setUsage(Usage usage)
{
	this->usage = usage;
}

void celestia::VertexBuffer::setDrawType(DrawType type)
{
	this->type = type;
}



/* TODO:
* Nuke current renderer and redo it.
* Guuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuh.
* Add a RenderPipeline class. It holds shaders, textures and such.
* Pass it with every draw command / func.
* Example: void guh:draw(const RendererHandler& renderer, const RenderPipeline& pipeline);
*/
void celestia::VertexBuffer::draw(const RendererHandler& renderer) const
{
	//renderer.drawSprite(quad, texture->getRawTexturePtr());
}



