#include "RendererHandler.hpp"
#include "Backend/VulkanAPI/RenderBack/BatchRender/BatchRender.hpp"
#include "Backend/VulkanAPI/RenderBack/TextRender/TextRender.hpp"
#include "Backend/VulkanAPI/RenderBack/DefaultRender/DefaultSingleRender.hpp"

#include "Backend/VulkanAPI/RenderBack/RendererHandler.hpp"
#include "Graphics/Drawable.hpp"
#include <vulkan/vulkan.h>
#include <memory>


static std::unique_ptr<celestia::RendererHandler> handler = nullptr;

celestia::RendererHandler& celestia::RendererHandler::getInstance()
{
	return *handler.get();
}

void celestia::RendererHandler::init(uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch)
{
	handler = std::make_unique<RendererHandler>(maxTexturesInShader,maxQuadsPerBatch);
}

void celestia::RendererHandler::cleanup()
{
	handler.reset();
}

celestia::RendererHandler::RendererHandler(uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch)
{
	coreRenderer = std::make_unique<Render>();
	batchSpriteRenderer = std::make_unique<BatchSpriteRender>(*coreRenderer, maxTexturesInShader, maxQuadsPerBatch);
	defaultSingleRenderer = std::make_unique<DefaultSingleRenderer>(*coreRenderer);
}

celestia::RendererHandler::~RendererHandler()
{
	coreRenderer->cleanUp();
}

void celestia::RendererHandler::setActiveRenderTarget(celestia::RenderTarget* target)
{
	activeTarget = target;
	coreRenderer->setRenderTarget(target);
}


celestia::RenderTarget* celestia::RendererHandler::getActiveRenderTarget() const
{
	return activeTarget;
}

void celestia::RendererHandler::draw(const Drawable& drawable) const
{
	drawable.draw(*this, nullptr);
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
	batchSpriteRenderer->drawQuad(position, size, color);
}

void celestia::RendererHandler::drawText(const std::vector<VertexBatch>& vertices, const int size, const Vec2& position,
  const Font_t& font, bool dirty, const int id) const
{
	if(batchTextRenderer)
	{
		batchTextRenderer->drawText(vertices, size, position, font, dirty, id);
	}
}

void celestia::RendererHandler::drawVertices(
  Mesh* meshPtr, const uint32_t amountToDraw, const RawTexture* texture) const
{
	defaultSingleRenderer->draw(meshPtr, amountToDraw, texture);
}

void celestia::RendererHandler::beginRenderPass() const
{
	const_cast<RendererHandler*>(this)->isRenderingActive = true;
	batchSpriteRenderer->beginBatch();
	if(batchTextRenderer)
	{
		batchTextRenderer->begin();
	}
	coreRenderer->beginRendering();
}

void celestia::RendererHandler::endRenderPass() const
{
	batchSpriteRenderer->endBatch();
	batchSpriteRenderer->flush();

	if(batchTextRenderer && batchTextRenderer->isActive())
	{
		batchTextRenderer->end();
		batchTextRenderer->flush();
	}

	coreRenderer->endRendering();
	const_cast<RendererHandler*>(this)->isRenderingActive = false;
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
	batchTextRenderer = std::make_unique<TextRender>(*coreRenderer, maxTextObjects, maxCharsPerBatch);
}
