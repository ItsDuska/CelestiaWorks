#pragma once
#include "RenderTarget.hpp"
#include "Texture.hpp"
#include <memory>

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

		CELESTIA_WORKS Vec2i getSize() const override;

		CELESTIA_WORKS const Texture* getTexture() const;
		CELESTIA_WORKS const RawTexture* getRawTexture() const;
		CELESTIA_WORKS void resize(Vec2i newSize);

	private:
		void activateAsRenderTarget() override;
		VkRenderPass getRenderPass() const override;
		VkFramebuffer getFramebuffer() const override;
	private:
		std::unique_ptr<FrameBuffer> frameBuffer;
		std::unique_ptr<Texture> m_texture;
		Vec2i size;
		bool hasDepth;

		void initialize();
		void cleanup();
	};
} // namespace celestia
