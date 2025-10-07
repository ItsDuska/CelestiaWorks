#pragma once
#include "Backend/VulkanAPI/RenderBack/VkRender/RenderBackend.hpp"
#include "Backend/VulkanAPI/Config/VulkanConfig.hpp"
#include "Backend/VulkanAPI/Core/Descriptor.hpp"

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
	};
} // namespace celestia