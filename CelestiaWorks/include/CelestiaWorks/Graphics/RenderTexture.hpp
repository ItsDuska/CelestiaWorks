#pragma once

#include "../System/Vector.hpp"
#include "System/export.hpp"
#include <memory>

namespace celestia
{
	struct RawTexture;
	class FrameBuffer;

	class RenderTexture
	{
	public:
		CELESTIA_WORKS RenderTexture(Vec2i size, bool hasDepthBuffer = false);

		CELESTIA_WORKS RenderTexture(const RenderTexture&) = delete;
		CELESTIA_WORKS RenderTexture& operator=(const RenderTexture&) = delete;
		CELESTIA_WORKS ~RenderTexture();

		CELESTIA_WORKS void beginRendering();
		CELESTIA_WORKS void endRendering();
		CELESTIA_WORKS void setClearColor(const Color& color);

		CELESTIA_WORKS const RawTexture* getTexture() const;
		CELESTIA_WORKS Vec2i getSize() const;

		CELESTIA_WORKS void resize(Vec2i newSize);
		CELESTIA_WORKS void saveToFile(const char* filepath);
		CELESTIA_WORKS void clear();

		CELESTIA_WORKS bool isValid() const;
		CELESTIA_WORKS bool isRendering() const;

	private:
		std::unique_ptr<FrameBuffer> frameBuffer;
		Vec2i size;
		Vec4 clearColor;
		bool rendering;
		bool hasDepth;

		friend class WindowHandle;
	};
} // namespace celestia