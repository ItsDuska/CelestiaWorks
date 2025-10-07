#include "Graphics/WindowHandle.hpp"

#ifdef _WIN32
#include "Backend/Window/Win32/Win32Window.hpp"
#elif __linux__
#include "Backend/Window/Wayland/WaylandWindow.hpp"
#endif
#include "Backend/Window/WindowContext.hpp"
#include "Backend/VulkanAPI/RenderBack/RendererHandler.hpp"
#include "Backend/VulkanAPI/Resources/FontReader.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Text.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/VertexBuffer.hpp"
#include <iostream>

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

	render = std::make_unique<RendererHandler>(maxTexturesInShader, maxQuadsPerBatch);
}

celestia::WindowHandle::~WindowHandle()
{
}

CELESTIA_WORKS void celestia::WindowHandle::draw(const Drawable& drawable) const
{
	render->draw(drawable);
}

CELESTIA_WORKS void celestia::WindowHandle::draw(const Drawable& drawable, RenderPipeline& pipeline) const
{
	render->draw(drawable, pipeline);
}

void celestia::WindowHandle::beginRenderPass() const
{
	render->beginRenderPass();
}

void celestia::WindowHandle::endRenderPass() const
{
	render->endRenderPass();
}

bool celestia::WindowHandle::isOpen() const
{
	return window->processMessages();
}

void celestia::WindowHandle::setClearColor(Color& color)
{
	render->setClearColor(color);
}

void celestia::WindowHandle::setFrameRateLimit(const int frameRate)
{
	render->setFrameRateLimit(frameRate);
}

void celestia::WindowHandle::createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch)
{
	render->createTextRenderer(maxTextObjects, maxCharsPerBatch);
}

CELESTIA_WORKS celestia::Vec2i celestia::WindowHandle::screenSpaceToWindowSpace(Vec2i& position)
{
	return window->screenSpaceToWindowSpace(position);
}
