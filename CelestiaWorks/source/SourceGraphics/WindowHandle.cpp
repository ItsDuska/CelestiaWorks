#include "Graphics/WindowHandle.hpp"

#ifdef _WIN32
#include "Backend/Window/Win32/Win32Window.hpp"
#elif __linux__
#include "Backend/Window/Wayland/WaylandWindow.hpp"
#endif
#include "Backend/Window/WindowContext.hpp"
#include "Backend/VulkanAPI/RenderBack/RendererHandler.hpp"


celestia::WindowHandle::WindowHandle(
  const Vec2i size, const char* name, uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch)
{
#ifdef _WIN32
	auto win = std::make_unique<Win32Window>(size, name);
#elif __linux__
	auto win = std::make_unique<WaylandWindow>(size, name);
#endif

	WindowContext::set(win.get());
	window = std::move(win);

	RendererHandler::init(maxTexturesInShader, maxQuadsPerBatch);
}

celestia::WindowHandle::~WindowHandle()
{
	RendererHandler::cleanup();
}

CELESTIA_WORKS void celestia::WindowHandle::draw(const Drawable& drawable) const
{
	RendererHandler::getInstance().draw(drawable);
}

CELESTIA_WORKS void celestia::WindowHandle::draw(const Drawable& drawable, RenderPipeline& pipeline) const
{
	RendererHandler::getInstance().draw(drawable,pipeline);
}

void celestia::WindowHandle::beginRenderPass()
{
	this->activateAsRenderTarget();
	RendererHandler::getInstance().beginRenderPass();
}

void celestia::WindowHandle::endRenderPass() const
{
	RendererHandler::getInstance().endRenderPass();
}

bool celestia::WindowHandle::isOpen() const
{
	return window->processMessages();
}

void celestia::WindowHandle::setClearColor(Color& color)
{
	RendererHandler::getInstance().setClearColor(color);
}

void celestia::WindowHandle::setFrameRateLimit(const int frameRate)
{
	RendererHandler::getInstance().setFrameRateLimit(frameRate);
}

void celestia::WindowHandle::createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch)
{
	RendererHandler::getInstance().createTextRenderer(maxTextObjects, maxCharsPerBatch);
}

CELESTIA_WORKS celestia::Vec2i celestia::WindowHandle::screenSpaceToWindowSpace(Vec2i& position)
{
	return window->screenSpaceToWindowSpace(position);
}

celestia::Vec2i celestia::WindowHandle::getSize() const
{
	return window->getWindowSize();
}

void celestia::WindowHandle::activateAsRenderTarget()
{
	// nullptr means that we are using the default swapchain.
	RendererHandler::getInstance().setActiveRenderTarget(nullptr);
}

VkRenderPass celestia::WindowHandle::getRenderPass() const
{
	// This should not be called for the WindowHandle.
	return nullptr;
}

VkFramebuffer celestia::WindowHandle::getFramebuffer() const
{
	// This should not be called for the WindowHandle.
	return nullptr;
}
