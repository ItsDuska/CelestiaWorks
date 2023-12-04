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
	class Window;

	class CELESTIA_WORKS WindowHandle
	{
	public:
		WindowHandle(const Vec2i size, const char* name);
		WindowHandle(const WindowHandle&) = delete;
		WindowHandle& operator = (const WindowHandle&) = delete;
		~WindowHandle();

	public:
		void draw() const;
		void beginRenderPass() const;
		void endRenderPass() const;
		bool isOpen() const;

	private:
		std::unique_ptr<Render> render;
		std::unique_ptr<Window> window;
	};
}
