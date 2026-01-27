#include "Graphics/RenderTarget.hpp"
#include "Backend/VulkanAPI/RenderBack/RendererHandler.hpp"
#include "Backend/VulkanAPI/RenderBack/VkRender/RenderBackend.hpp"
#include "Graphics/Drawable.hpp"
#include "Graphics/RenderPipeline.hpp"


void celestia::RenderTarget::draw(const Drawable& drawable) const
{
	RendererHandler::getInstance().draw(drawable);
}

void celestia::RenderTarget::draw(const Drawable& drawable, RenderPipeline& pipeline) const
{
	RendererHandler::getInstance().draw(drawable, pipeline);
}

void celestia::RenderTarget::beginRenderPass()
{
	activateAsRenderTarget();
	RendererHandler::getInstance().beginRenderPass();
}

void celestia::RenderTarget::endRenderPass()
{
	RendererHandler::getInstance().endRenderPass();
}

void celestia::RenderTarget::setClearColor(Color& color)
{
	RendererHandler::getInstance().setClearColor(color);
}
