#pragma once
#include <vulkan/vulkan.h>
#include "CelestiaVulkanTypes.hpp"

namespace celestia::vk
{
	class Image
	{
	public:
		Image();
		~Image();

		// Texture loading methods (legacy)
		static RawTexture createTextureImage(const char* filepath, Vec2i& size, bool isDefaultTexture = false);
		static void createTextureFromBuffer(const void* bufferptr, const VkDeviceSize& bufferSize, const Vec2i& size,
		  RawTexture& texture, VkFormat format);
		static void deleteTextureImage(RawTexture& texture);

		// Compute shader image methods
		static RawTexture createStorageImage(Vec2i size, VkFormat format);
		static RawTexture createStorageImageForSampling(Vec2i size, VkFormat format);
		static RawTexture
		createStorageImageWithData(Vec2i size, VkFormat format, const void* data, VkDeviceSize dataSize);
		static void copyImageToBuffer(const RawTexture& image, VkBuffer buffer, Vec2i imageSize);
		static void transitionImageLayoutForCompute(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

	public:
		RawTexture defaultTexture;
		VkSampler textureSampler;

	public:
		// Core image creation methods
		static void createImage(Vec2i imageSize, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		  VkMemoryPropertyFlags properties, AllocatedImage& image);
		static void transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void copyBufferToImage(VkBuffer buffer, VkImage image, Vec2u imageSize);
		static VkImageView
		createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
		static VkImageView createImageViewForStorage(VkImage image, VkFormat format);

		static VkSampler
		createTextureSampler(VkFilter magFilter = VK_FILTER_LINEAR, VkFilter minFilter = VK_FILTER_LINEAR,
		  VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		  VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		  VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, bool enableAnisotropy = false,
		  float maxAnisotropy = 1.0f, VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		  VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR, float mipLodBias = 0.0f, float minLod = 0.0f,
		  float maxLod = 0.0f);
	};

}; // namespace celestia::vk
