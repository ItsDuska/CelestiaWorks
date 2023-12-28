#pragma once

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include <memory>
#include "celestiaTypes/CelestiaTypes.h"


namespace celestia
{
	class Render;
	class BatchRender;
	class Window;
	class Sprite;

	class WindowHandle
	{
	public:
		CELESTIA_WORKS WindowHandle(const Vec2i size, const char* name);
		CELESTIA_WORKS WindowHandle(const WindowHandle&) = delete;
		CELESTIA_WORKS WindowHandle& operator = (const WindowHandle&) = delete;
		CELESTIA_WORKS ~WindowHandle();

	public:
		CELESTIA_WORKS void draw(const Sprite& sprite) const;
		CELESTIA_WORKS void beginRenderPass() const;
		CELESTIA_WORKS void endRenderPass() const;
		CELESTIA_WORKS bool isOpen() const;
		CELESTIA_WORKS void setClearColor(Color& color);

	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<BatchRender> render;	
	};
}
