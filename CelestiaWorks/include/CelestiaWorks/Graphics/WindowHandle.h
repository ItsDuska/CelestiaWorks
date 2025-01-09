#pragma once
#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include <memory>
#include "../System/CelestiaTypes.h"

namespace celestia
{
	class Render;
	class RendererHandler;
	class Window;
	class Sprite;
	class Text;
	class VertexBuffer;

	class WindowHandle
	{
	public:
		CELESTIA_WORKS WindowHandle(const Vec2i size, const char* name, uint32_t maxTexturesInShader = 5u, uint32_t maxQuadsPerBatch = 100);
		CELESTIA_WORKS WindowHandle(const WindowHandle&) = delete;
		CELESTIA_WORKS WindowHandle& operator = (const WindowHandle&) = delete;
		CELESTIA_WORKS ~WindowHandle();

	public:
		CELESTIA_WORKS void draw(const Sprite& sprite) const;
		CELESTIA_WORKS void draw(Text& text) const;
		CELESTIA_WORKS void draw(VertexBuffer& buffer) const;
		CELESTIA_WORKS void beginRenderPass() const;
		CELESTIA_WORKS void endRenderPass() const;
		CELESTIA_WORKS bool isOpen() const;
		CELESTIA_WORKS void setClearColor(Color& color);
		CELESTIA_WORKS void setFrameRateLimit(const int frameRate);
		CELESTIA_WORKS void createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch);

		CELESTIA_WORKS Vec2i screenSpaceToWindowSpace(Vec2i& position);

	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<RendererHandler> render;
	};
}
