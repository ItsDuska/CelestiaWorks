#include "RendererHandler.h"
#include "backend/vulkanAPI/renderBack/batchRender/BatchRender.h"
#include "backend/vulkanAPI/renderBack/textRender/TextRender.h"
#include "backend/vulkanAPI/renderBack/defaultRender/DefaultSingleRender.h"

#include "backend/window/Window.h"
#include "backend/vulkanAPI/renderBack/RendererHandler.h"
#include "Graphics/Drawable.h"

celestia::RendererHandler::RendererHandler(Window& window, uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch)
{
	coreRenderer = std::make_unique<Render>(window);
	batchSpriteRenderer = std::make_unique<BatchSpriteRender>(*coreRenderer, maxTexturesInShader, maxQuadsPerBatch);
	defaultSingleRenderer = std::make_unique<DefaultSingleRenderer>(*coreRenderer);
}

celestia::RendererHandler::~RendererHandler()
{
	coreRenderer->cleanUp();
}

void celestia::RendererHandler::draw(const Drawable& drawable) const
{
	drawable.draw(*this,nullptr);
}

void celestia::RendererHandler::draw(const Drawable& drawable, RenderPipeline& pipeline) const
{
	drawable.draw(*this, &pipeline);
}

void celestia::RendererHandler::drawSprite(const VertexPositions* quad, const RawTexture* texture) const
{
	batchSpriteRenderer->drawQuad(quad, texture);
}

void celestia::RendererHandler::drawQuad(const Vec2& position, const Vec2& size, const Vec3& color) const
{
	batchSpriteRenderer->drawQuad(position, size,color);
}

void celestia::RendererHandler::drawText(const std::vector<VertexBatch>& vertices, const int size, const Vec2& position, const Font_t& font, bool dirty,const int id) const
{
	if (batchTextRenderer)
	{
		batchTextRenderer->drawText(vertices, size, position, font, dirty, id);
	}
}

void celestia::RendererHandler::drawVertices(Mesh* meshPtr, const uint32_t amountToDraw, const RawTexture* texture) const
{
	defaultSingleRenderer->draw(meshPtr, amountToDraw, texture);
}

void celestia::RendererHandler::beginRenderPass() const
{
	batchSpriteRenderer->beginBatch();
	if (batchTextRenderer)
	{
		batchTextRenderer->begin();
	}
	coreRenderer->beginRendering();
}

void celestia::RendererHandler::endRenderPass() const
{
	batchSpriteRenderer->endBatch();
	batchSpriteRenderer->flush();

	if (batchTextRenderer && batchTextRenderer->isActive())
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

void celestia::RendererHandler::createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch)
{
	batchTextRenderer = std::make_unique<TextRender>(*coreRenderer,maxTextObjects,maxCharsPerBatch);
}
