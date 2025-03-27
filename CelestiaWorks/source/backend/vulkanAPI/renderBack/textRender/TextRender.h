#pragma once
#include "backend/vulkanAPI/renderBack/defaultRender/RenderBackend.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"
#include "backend/vulkanAPI/core/Descriptor.h"
#include <unordered_map>
#include <stack>

/*
* What to do:
* When text draw is called. Add the item to the n.th buffer. We should 
* have n amount of buffers that stores our drawInfo. We should only actually store:
*	- Texture ptr.
*	- Allocated buffer ptr.
*	- glyph buffer for that allocated buffer.
*	
* 
*	Use stack as an indicator for that to draw in this frame. If we use new font, 
*	 then add it to the stack. At the end of the frame, we pop the stack clean.
*	 
*/

namespace celestia
{
	constexpr uint32_t MAX_FONT_BUFFERS = 3u;

	struct TextBuffer_t // keksi parempi nimi
	{
		std::vector<Vertex> glyphBuffer;
		uint32_t vertexCount;
		uint32_t indexCount;

		Mesh *mesh;
	};



	struct Font_t;

	class TextRender
	{
	public:
		TextRender(Render& render, uint32_t maxTextObjects, uint32_t maxCharsPerBatch);
		~TextRender();
		void begin();
		void end();
		void flush();

		void drawText(const std::vector<Vertex>& vertices, const int size, const Vec2& position, const Font_t& font,bool dirty, const int id);

		bool isActive();
	private:
		Render& render;
		DrawInfo info;
		std::unique_ptr<Descriptor> descriptors;

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

		const uint32_t MAX_TEXT_COUNT;
		const uint32_t MAX_VERTEX_COUNT_PER_BATCH;
		const uint32_t MAX_INDEX_COUNT_PER_BATCH;

		// TESTING NEW BUFFER THINGY

		//TextBuffer_t fontTypeBuffers[MAX_FONT_BUFFERS];
		//RawTexture* fontPtrKeyBuffer[MAX_FONT_BUFFERS];
		//std::unordered_map<uint32_t, uint32_t> fontTypeBufferMapKeys;

		//uint32_t lastIndexInMap;

	};
}




