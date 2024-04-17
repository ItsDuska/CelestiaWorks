#pragma once
#include <array>
#include "backend/vulkanAPI/renderBack/defaultRender/RenderBackend.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"

namespace celestia
{
	class Window;

	class BatchSpriteRender
	{
	public:
		BatchSpriteRender(Render& render);
		~BatchSpriteRender();

		void beginBatch();
		void endBatch();

		void flush();

		void drawQuad(const Vec2& position,
			const Vec2& size, const Vec3& color);

		void drawQuad(const VertexPositions* quad, const RawTexture* texture);

	private:
		Render& render;
		DrawInfo info;

		std::vector<Vertex> quadBuffer; 
		int vertexCount;
		int indexCount;
		std::array<int, NUMBER_OF_TEXTURE_IN_SHADER> textureSlots;
		VkImageView textures[NUMBER_OF_TEXTURE_IN_SHADER]{};
		int textureSlotIndex;
	};
}
