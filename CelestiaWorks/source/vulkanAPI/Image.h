#pragma once
#include <vulkan/vulkan.h>
#include "CelestiaVulkanTypes.h"

namespace celestia
{
	class Device;
	class Buffer;

	class Image
	{
	public:
		Image(Device& device,Buffer& buffer);
		~Image();

		void createTextureImage(const char* filepath,AllocatedImage& image);
	public:
		VkImageView textureImageView;
		VkSampler textureSampler;

	private:
		void createImage(Vec2i imageSize, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, AllocatedImage& image);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, Vec2u imageSize);
		VkImageView createImageView(VkImage image, VkFormat format);
		void createTextureSampler();

	private:
		Device& device;
		Buffer& buffer;

		AllocatedImage defaultTexture;
	};

}