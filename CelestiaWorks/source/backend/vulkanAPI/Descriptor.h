#pragma once
#include "backend/vulkanAPI/config/VulkanConfig.h"

#include <vulkan/vulkan.h>
#include <vector>

constexpr int MAX_WRITES = 2;
constexpr int TEXTURE_INDEX = 0;
constexpr int UNIFORM_BUFFER_INDEX = 1;


namespace celestia
{
	class Buffer;
	class Image;

	class Descriptor
	{
	public:
		Descriptor(Buffer& buffer, Image& image);
		~Descriptor();

		VkDescriptorSetLayout& getDescriptorSetLayout() { return globalSetLayout; }

		VkDescriptorPool& getDescriptorPool() { return descriptorPool; }

		VkDescriptorSet& getDescriptorSet(int currentFrame);

		void updateDescriptorSetTextures(const VkImageView* view, const VkSampler sampler,
			const int bindingSlot, const int count, const int currentFrame);

		void uppdateDescriporSetUniformBuffers(const VkBuffer* buffer,const VkDeviceSize size,
			const int bindingSlot,const int count, const int currentFrame);

		void updateAllDescriptorSets();
	private:
		void createDescriptors();
	private:

		VkDescriptorSetLayout globalSetLayout;
		VkDescriptorPool descriptorPool;

		std::vector<VkDescriptorSet> descriptorSets;

		VkDescriptorBufferInfo bufferInfo[NUMBER_OF_UNIFORM_BUFFERS];
		VkDescriptorImageInfo imageInfo[NUMBER_OF_TEXTURE_IN_SHADER];
		std::vector<VkWriteDescriptorSet> writes;

		Buffer& buffer;
		Image& image;
	};

}
