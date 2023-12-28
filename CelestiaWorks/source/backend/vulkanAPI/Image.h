#pragma once
#include <vulkan/vulkan.h>
#include "celestiaTypes/CelestiaVulkanTypes.h"

namespace celestia
{
	class Image
	{
	public:
		Image();
		~Image();

		static RawTexture createTextureImage(const char* filepath, Vec2i* size);
		static void deleteTextureImage(RawTexture& texture);
	public:
		RawTexture defaultTexture;
		VkSampler textureSampler;
	private:
		static void createImage(Vec2i imageSize, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, AllocatedImage& image);
		static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void copyBufferToImage(VkBuffer buffer, VkImage image, Vec2u imageSize);
		static VkImageView createImageView(VkImage image, VkFormat format);
		void createTextureSampler(); // Muuta ett‰ k‰ytet‰‰n pixel arvoja eik‰ 0-1 arvoja.
	};

}