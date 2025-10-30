#pragma once

#include <memory>
#include "../System/CelestiaTypes.hpp"

namespace celestia
{
	class Render;
	class RendererHandler;
	class PlatformWindow;

	class Drawable;
	class RenderPipeline;

	class WindowHandle
	{
	public:
		CELESTIA_WORKS WindowHandle(
		  const Vec2i size, const char* name, uint32_t maxTexturesInShader = 5u, uint32_t maxQuadsPerBatch = 100);
		CELESTIA_WORKS WindowHandle(const WindowHandle&) = delete;
		CELESTIA_WORKS WindowHandle& operator=(const WindowHandle&) = delete;
		CELESTIA_WORKS ~WindowHandle();

	public:
		CELESTIA_WORKS void draw(const Drawable& drawable) const;
		CELESTIA_WORKS void draw(const Drawable& drawable, RenderPipeline& pipeline) const;

		CELESTIA_WORKS void beginRenderPass() const;
		CELESTIA_WORKS void endRenderPass() const;
		CELESTIA_WORKS bool isOpen() const;
		CELESTIA_WORKS void setClearColor(Color& color);
		CELESTIA_WORKS void setFrameRateLimit(const int frameRate);
		CELESTIA_WORKS void createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch);

		CELESTIA_WORKS Vec2i screenSpaceToWindowSpace(Vec2i& position);

	private:
		std::unique_ptr<PlatformWindow> window;
		std::unique_ptr<RendererHandler> render;
	};
} // namespace celestia
