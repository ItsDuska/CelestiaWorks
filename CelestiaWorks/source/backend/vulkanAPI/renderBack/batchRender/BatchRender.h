#pragma once
#include <array>
#include "backend/vulkanAPI/renderBack/vkRender/RenderBackend.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"

namespace celestia
{
	class Window;
	class Descriptor;

	class BatchSpriteRender
	{
	public:
		BatchSpriteRender(Render& render, uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch);
		~BatchSpriteRender();

		void beginBatch();
		void endBatch();

		void flush();

		void drawQuad(const Vec2& position,
			const Vec2& size, const Vec3& color);

		void drawQuad(const VertexPositions* quad, const RawTexture* texture);

		Material getMaterial() const;
		FullDescriptorSet getDescriptors();

	private:
		Render& render;
		DrawInfo info;

		std::vector<VertexBatch> quadBuffer; 
		int vertexCount;
		int indexCount;
		std::array<int, NUMBER_OF_TEXTURE_IN_SHADER> textureSlots;
		VkImageView textures[NUMBER_OF_TEXTURE_IN_SHADER]{};
		std::unique_ptr<Descriptor> descriptors;
		VkDescriptorSet set[MAX_FRAMES_IN_FLIGHT]; // bit goofy but we go with this one for now... :p
VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT]; // Store descriptor sets for each frame
		int textureSlotIndex;


		// CONFIGURABLE STUFF || en kirjottanu varmaa tota oikein :D

		const uint32_t MAX_TEXTURES_IN_SHADER;
		const uint32_t MAX_QUAD_COUNT;
		const uint32_t MAX_VERTEX_COUNT_PER_BATCH;
		const uint32_t MAX_INDEX_COUNT_PER_BATCH;

	};


	
}
