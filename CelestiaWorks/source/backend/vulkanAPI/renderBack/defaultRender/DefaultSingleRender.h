#pragma once
#include "backend/vulkanAPI/renderBack/vkRender/RenderBackend.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"
#include "backend/vulkanAPI/core/Descriptor.h"



namespace celestia
{
	class DefaultSingleRenderer
	{
	public:
		DefaultSingleRenderer(Render& render);
		~DefaultSingleRenderer();

		void draw(Mesh* mesh, const uint32_t amountToDraw, const RawTexture* texture);

	private:
		Render& render;
		DrawInfo info;
		Descriptor descriptor;
		VkDescriptorSet set[MAX_FRAMES_IN_FLIGHT]{};
	};
}