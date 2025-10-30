#pragma once
#include "RenderTarget.hpp"

namespace celestia
{
	struct RawTexture;
	class FrameBuffer;

	class RenderTexture : public RenderTarget
	{
	public:
		CELESTIA_WORKS RenderTexture(Vec2i size, bool hasDepthBuffer = false);
		CELESTIA_WORKS RenderTexture(const RenderTexture&) = delete;
		CELESTIA_WORKS RenderTexture& operator=(const RenderTexture&) = delete;
		CELESTIA_WORKS ~RenderTexture() override;

		// RenderTarget interface implementation
		// CELESTIA_WORKS Vec2i getSize() const;
		// CELESTIA_WORKS bool isValid() const;

		// RenderTexture-specific features
		CELESTIA_WORKS const RawTexture* getTexture() const;
		CELESTIA_WORKS void resize(Vec2i newSize);
		CELESTIA_WORKS void saveToFile(const char* filepath);

		// Utility methods
		// CELESTIA_WORKS bool hasDepthBuffer() const;
		// CELESTIA_WORKS VkFormat getFormat() const;

	private:
		std::unique_ptr<FrameBuffer> frameBuffer;
		Vec2i size;
		bool hasDepth;

		void initialize();
		void cleanup();
	};
} // namespace celestia