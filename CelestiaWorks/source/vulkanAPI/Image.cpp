#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

#include "Device.h"
#include "Buffer.h"
#include "CommandBuffer.h"


/*
TODO:
Kirjota tää uudelleen, jotta pystyisit luomaan uusia tekstuureja käyttämällä vain tekstuurin path sijaintia.
Myös mahdollisuus luoda black white 1x1 tekstuuri default tekstuuriks jos joku kuolee.
*/

constexpr const char* TEMP_TEXTURE_PATH = "../assets/TempAsset1.jpg"; // TODO: poista tää


celestia::Image::Image(Device& device,Buffer& buffer)
	:device{device}, buffer{buffer}
{
	createTextureImage(TEMP_TEXTURE_PATH, defaultTexture);
	textureImageView = createImageView(defaultTexture.image, VK_FORMAT_R8G8B8A8_SRGB);
	createTextureSampler();
}

celestia::Image::~Image()
{
	vkDestroySampler(device.device, textureSampler, nullptr);
	vkDestroyImageView(device.device, textureImageView, nullptr);
}

void celestia::Image::createImage(Vec2i imageSize, VkFormat format, VkImageTiling tiling,
	VkImageUsageFlags usage, VkMemoryPropertyFlags properties, AllocatedImage& image)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(imageSize.x);
	imageInfo.extent.height = static_cast<uint32_t>(imageSize.y);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	if (vkCreateImage(device.device, &imageInfo, nullptr, &image.image) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image!\n");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device.device, image.image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = buffer.findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device.device, &allocInfo, nullptr, &image.memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(device.device, image.image, image.memory, 0);

}

void celestia::Image::createTextureImage(const char* filepath,AllocatedImage& image)
{
	Vec2i texSize{};
	int texChannels;
	stbi_uc* pixels = stbi_load(filepath, &texSize.x, &texSize.y, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texSize.x * texSize.y * 4;

	if (!pixels)
	{
		throw std::runtime_error("Failed to load texture image!\n");
	}

	AllocatedBuffer stagingBuffer = buffer.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(device.device, stagingBuffer.memory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device.device, stagingBuffer.memory);

	stbi_image_free(pixels);

	createImage(texSize,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		image
	);

	transitionImageLayout(image.image,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	);

	copyBufferToImage(stagingBuffer.buffer,
		image.image,
		{ static_cast<uint32_t>(texSize.x), static_cast<uint32_t>(texSize.y) }
	);

	transitionImageLayout(image.image,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	);

	vkDestroyBuffer(device.device, stagingBuffer.buffer, nullptr);
	vkFreeMemory(device.device, stagingBuffer.memory, nullptr);

	device.deletionQueue.pushFunction([=]() {
		vkDestroyImage(device.device, image.image, nullptr);
		vkFreeMemory(device.device, image.memory, nullptr);
		}
	);
}

void celestia::Image::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer cmdBuffer = beginSingleTimeCommands(device.commandPool, device.device);

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}


	vkCmdPipelineBarrier(
		cmdBuffer,
		sourceStage,
		destinationStage,
		0,
		0, 
		nullptr,
		0, 
		nullptr,
		1, 
		&barrier
	);


	endSingleTimeCommands(device.graphicsQueue, device.commandPool, device.device, cmdBuffer);
}

void celestia::Image::copyBufferToImage(VkBuffer buffer, VkImage image, Vec2u imageSize)
{
	VkCommandBuffer cmdBuffer = beginSingleTimeCommands(device.commandPool, device.device);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		imageSize.x,
		imageSize.y,
		1
	};

	vkCmdCopyBufferToImage(
		cmdBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	endSingleTimeCommands(device.graphicsQueue, device.commandPool, device.device, cmdBuffer);
}

VkImageView celestia::Image::createImageView(VkImage image, VkFormat format)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create texture image view!\n");
	}

	return imageView;
}

void celestia::Image::createTextureSampler()
{
	VkSamplerCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	info.magFilter = VK_FILTER_LINEAR;
	info.minFilter = VK_FILTER_LINEAR;
	info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.anisotropyEnable = VK_TRUE;

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(device.physicalDevice, &properties);

	info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	info.unnormalizedCoordinates = VK_FALSE;
	info.compareEnable = VK_FALSE;
	info.compareOp = VK_COMPARE_OP_ALWAYS;
	info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	info.mipLodBias = 0.f;
	info.minLod = 0.f;
	info.maxLod = 0.f;

	if (vkCreateSampler(device.device, &info, nullptr, &textureSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create texture sampler!\n");
	}
}
