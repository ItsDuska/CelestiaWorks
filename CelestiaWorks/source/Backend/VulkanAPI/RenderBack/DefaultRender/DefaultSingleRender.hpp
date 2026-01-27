#pragma once
#include "Backend/VulkanAPI/RenderBack/VkRender/RenderBackend.hpp"
#include "Backend/VulkanAPI/Config/VulkanConfig.hpp"
#include "Vulkan/Descriptor.hpp"

namespace celestia
{
	class DefaultSingleRenderer
	{
	public:
		DefaultSingleRenderer(Render& render);
		~DefaultSingleRenderer();

		void draw(vk::Mesh* mesh, const uint32_t amountToDraw, const vk::RawTexture* texture);

	private:
		Render& render;
		vk::DrawInfo info;
		vk::Descriptor descriptor;
	};
} // namespace celestia
