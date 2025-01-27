#include "Graphics/WindowHandle.h"
#include "backend/window/Window.h"
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
	: window{ std::make_unique<Window>(size, name) }
{
	render = std::make_unique<RendererHandler>(
		*window,
		maxTexturesInShader,
		maxQuadsPerBatch
	);
}

celestia::WindowHandle::~WindowHandle()
{
}

CELESTIA_WORKS void celestia::WindowHandle::draw(const Drawable& drawable) const
{
	return render->draw(drawable);
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
