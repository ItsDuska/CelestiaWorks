#include "RendererHandler.h"
#include "backend/window/Window.h"
#include "backend/vulkanAPI/renderBack/RendererHandler.h"

celestia::RendererHandler::RendererHandler(Window& window)
{
	coreRenderer = std::make_unique<Render>(window);
	batchSpriteRenderer = std::make_unique<BatchSpriteRender>(*coreRenderer);
	batchTextRenderer = std::make_unique<TextRender>(*coreRenderer);
}

celestia::RendererHandler::~RendererHandler()
{
	coreRenderer->cleanUp();
}

void celestia::RendererHandler::drawSprite(const VertexPositions* quad, const RawTexture* texture)
{
	batchSpriteRenderer->drawQuad(quad, texture);
}

void celestia::RendererHandler::drawQuad(const Vec2& position, const Vec2& size, const Vec3& color) const
{
	batchSpriteRenderer->drawQuad(position, size,color);
}

//TODO:
void celestia::RendererHandler::drawText(const std::vector<Vertex>& vertices, const int size, const Vec2& position, const Font_t& font, bool dirty,const int id)
{
	batchTextRenderer->drawText(vertices, size, position, font, dirty,id);
	//batchTextRenderer->draw(text.data, text.position, text.size, text.font);
}

void celestia::RendererHandler::beginRenderPass() const
{
	batchSpriteRenderer->beginBatch();
	batchTextRenderer->begin();
	coreRenderer->beginRendering();
}

void celestia::RendererHandler::endRenderPass() const
{
	batchSpriteRenderer->endBatch();
	batchSpriteRenderer->flush();

	if (batchTextRenderer->isActive())
	{
		batchTextRenderer->end();
		batchTextRenderer->flush();
	}
	
	coreRenderer->endRendering();
}


void celestia::RendererHandler::setClearColor(Color& color)
{
	coreRenderer->setClearColor(color);
}

void celestia::RendererHandler::setFrameRateLimit(const int frameRate)
{
	coreRenderer->setFramerateLimit(frameRate);
}
