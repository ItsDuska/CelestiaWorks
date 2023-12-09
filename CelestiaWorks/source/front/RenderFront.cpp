#include "RenderFront.h"
#include "window/Window.h"
#include "vulkanAPI/RenderBackend.h"
#include <iostream>


celestia::WindowHandle::WindowHandle(const Vec2i size, const char* name)
	: window{ std::make_unique<Window>(size, name) },
	  render{ std::make_unique<Render>(*window) }
{
	//window = std::make_unique<Window>(size, name);
	//render = std::make_unique<Render>(*window);
}


celestia::WindowHandle::~WindowHandle()
{
	//window.release();
	//render.release();
}


//TODO: make this actually draw any gameObjects.
void celestia::WindowHandle::draw() const
{
	render->draw();
}

void celestia::WindowHandle::beginRenderPass() const
{
	render->beginRendering();
}

void celestia::WindowHandle::endRenderPass() const
{
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
