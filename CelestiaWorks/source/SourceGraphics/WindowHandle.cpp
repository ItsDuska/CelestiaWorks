#include "Graphics/WindowHandle.h"

#ifdef _WIN32
#include "backend/window/Win32/Win32Window.h"
#elif __linux__
#include "backend/window/Wayland/WaylandWindow.h"
#endif
#include "backend/window/WindowContext.h"
#include "backend/vulkanAPI/renderBack/RendererHandler.h"
#include "Graphics/Sprite.h"
#include "Graphics/Text.h"
#include "Graphics/Font.h"
#include "backend/vulkanAPI/resources/FontReader.h"
#include "Graphics/VertexBuffer.h"
#include <iostream>


celestia::WindowHandle::WindowHandle(const Vec2i size,
	const char* name,
	uint32_t maxTexturesInShader,
	uint32_t maxQuadsPerBatch)
{

#ifdef _WIN32
	auto win = std::make_unique<Win32Window>(size, name);
#elif __linux__
	auto win = std::make_unique<WaylandWindow>(size, name);
#endif

	WindowContext::set(win.get());
	window = std::move(win);

	render = std::make_unique<RendererHandler>(
		maxTexturesInShader,
		maxQuadsPerBatch
	);
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
