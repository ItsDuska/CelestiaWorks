#pragma once
#include <memory>
#include "backend/vulkanAPI/renderBack/batchRender/BatchRender.h"
#include "backend/vulkanAPI/renderBack/textRender/TextRender.h"


namespace celestia
{
	class TextRender;
	class Window;
	class Text;

	class RendererHandler
	{
	public:
		RendererHandler(Window& window, uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch);
		~RendererHandler();

		void drawSprite(const VertexPositions* quad, const RawTexture* texture);
		void drawQuad(const Vec2& position, const Vec2& size, const Vec3& color) const;
		void drawText(const std::vector<Vertex>& vertices, const int size, const Vec2& position, const Font_t& font, bool dirty, const int id);

		void beginRenderPass() const;
		void endRenderPass() const;
		void setClearColor(Color& color);
		void setFrameRateLimit(const int frameRate);


		//TEXT RENDERING
		void createTextRenderer(uint32_t maxTextObjects, uint32_t maxCharsPerBatch);

	private:
		std::unique_ptr<Render> coreRenderer; //t‰t‰ heitell‰‰n molemmille render classeille

		std::unique_ptr<BatchSpriteRender> batchSpriteRenderer;
		std::unique_ptr<TextRender> batchTextRenderer;
		//std::unique_ptr<TextRender> batchTextRenderer;
		//std::unique_ptr<TextRender> batchTextRenderer;
	};

}

