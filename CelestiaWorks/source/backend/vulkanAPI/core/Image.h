#pragma once
#include <vulkan/vulkan.h>
#include "CelestiaVulkanTypes.h"


namespace celestia
{
	class Image
	{
	public:
		Image();
		~Image();

		static RawTexture createTextureImage(const char* filepath, Vec2i& size, bool isDefaultTexture = false);
		static void createTextureFromBuffer(const void* bufferptr, const VkDeviceSize& bufferSize, const Vec2i& size, RawTexture& texture, VkFormat format);
		static void deleteTextureImage(RawTexture& texture);
	public:
		RawTexture defaultTexture;
		VkSampler textureSampler;
	private:
		static void createImage(Vec2i imageSize, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, AllocatedImage& image);
		static void transitionImageLayout(VkImage image,VkImageLayout oldLayout, VkImageLayout newLayout);
		static void copyBufferToImage(VkBuffer buffer, VkImage image, Vec2u imageSize);
		static VkImageView createImageView(VkImage image, VkFormat format);
		
		void createTextureSampler(); // Muuta ett‰ k‰ytet‰‰n pixel arvoja eik‰ 0-1 arvoja.
	};

}