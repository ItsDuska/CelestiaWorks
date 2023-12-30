#pragma once
#include <array>
#include "backend/vulkanAPI/renderBack/defaultRender/RenderBackend.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"

namespace celestia
{
	class Window;

	class BatchRender :public Render
	{
	public:
		BatchRender(Window& window);
		~BatchRender();

		void beginBatch();
		void endBatch();

		void flush();

		void drawQuad(const Vec2& position,
			const Vec2& size, const Vec3& color);

		void drawQuad(const VertexPositions* quad, const RawTexture* texture);

	private:
		std::vector<Vertex> quadBuffer;
		Mesh mesh; 
		int vertexCount;
		int indexCount;
		std::array<int, NUMBER_OF_TEXTURE_IN_SHADER> textureSlots;
		VkImageView textures[NUMBER_OF_TEXTURE_IN_SHADER]{};
		int textureSlotIndex;
	};
}
