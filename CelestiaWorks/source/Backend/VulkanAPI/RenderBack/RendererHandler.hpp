#pragma once
#include "System/CelestiaTypes.hpp"
#include <vector>
#include <memory>

namespace celestia
{
	class TextRender;
	class Text;
	class Drawable;
	class Render;
	class RenderPipeline;
	class BatchSpriteRender;
	class TextRender;
	class DefaultSingleRenderer;
	struct RawTexture;
	struct Font_t;
	struct Mesh;

	class RendererHandler
	{
	public:
		RendererHandler(uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch);
		~RendererHandler();

		void draw(const Drawable& drawable) const;
		void draw(const Drawable& drawable, RenderPipeline& pipeline) const;

		// Bacth rendering functions
		void drawSprite(const VertexPositions* quad, const RawTexture* texture) const;
		void drawQuad(const Vec2& position, const Vec2& size, const Vec3& color) const;
		void drawText(const std::vector<VertexBatch>& vertices, const int size, const Vec2& position,
		  const Font_t& font, bool dirty, const int id) const;

		// normal draw call. Will draw it instantly.
		void drawVertices(Mesh* meshPtr, const uint32_t amountToDraw, const RawTexture* texture) const;

		void beginRenderPass() const;
		void endRenderPass() const;
		void setClearColor(Color& color);
		void setFrameRateLimit(const int frameRate);

		// TEXT RENDERING
		void createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch);

	private:
		std::unique_ptr<Render> coreRenderer; // t�t� heitell��n molemmille render classeille

		std::unique_ptr<BatchSpriteRender> batchSpriteRenderer;
		std::unique_ptr<TextRender> batchTextRenderer;
		std::unique_ptr<DefaultSingleRenderer> defaultSingleRenderer;
		// std::unique_ptr<TextRender> batchTextRenderer;
		// std::unique_ptr<TextRender> batchTextRenderer;
	};

} // namespace celestia
