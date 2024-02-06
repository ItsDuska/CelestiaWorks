#include "Graphics/WindowHandle.h"
#include "backend/window/Window.h"
#include "backend/vulkanAPI/renderBack/batchRender/BatchRender.h"
#include "Graphics/Sprite.h"
#include <iostream>


celestia::WindowHandle::WindowHandle(const Vec2i size, const char* name)
	: window{ std::make_unique<Window>(size, name) }
{
	render = std::make_unique<BatchRender>(*window);
}

celestia::WindowHandle::~WindowHandle()
{
}

void celestia::WindowHandle::draw(const Sprite& sprite) const
{
	render->drawQuad(sprite.quad, sprite.getTexture()->pixels.get());
}

void celestia::WindowHandle::beginRenderPass() const
{
	render->beginBatch();
	render->beginRendering();
}

void celestia::WindowHandle::endRenderPass() const
{
	render->endBatch();
	render->flush();
	render->endRendering();
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
	render->setFramerateLimit(frameRate);
}
