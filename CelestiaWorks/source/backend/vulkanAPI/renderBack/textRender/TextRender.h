#pragma once
#include "backend/vulkanAPI/renderBack/defaultRender/RenderBackend.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"
#include "backend/vulkanAPI/core/Descriptor.h"

namespace celestia
{
	struct Font_t;

	class TextRender
	{
	public:
		TextRender(Render& render);
		~TextRender();
		void begin();
		void end();
		void flush();

		void drawText(const std::vector<Vertex>& vertices, const int size, const Vec2& position, const Font_t& font,bool dirty, const int id);

		bool isActive();
	private:
		Render& render;
		DrawInfo info;
		std::unique_ptr<DescriptorFactory> descriptors;

		const RawTexture* currentTexturePtr;
		VkDescriptorSet set[MAX_FRAMES_IN_FLIGHT];
		Material defaultMaterial;

		AllocatedBuffer storageBuffer[MAX_FRAMES_IN_FLIGHT];
		const VkDeviceSize bufferSize;

		std::vector<Vec2Aligned> transformationBuffer;
		int transformationIndexCounter;
		//std::vector<int> idBuffer;

		std::vector<Vertex> glyphBuffer;
		int vertexCount;
		int indexCount;

		bool needsUpdate;
		bool active;


	};
}




