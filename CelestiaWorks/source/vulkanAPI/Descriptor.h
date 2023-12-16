#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace celestia
{
	class Device;
	class Buffer;
	class Image;

	class Descriptor
	{
	public:
		Descriptor(Device& deive, Buffer& buffer, Image& image);
		~Descriptor();

		VkDescriptorSetLayout& getDescriptorSetLayout() { return globalSetLayout; }
		VkDescriptorPool& getDescriptorPool() { return descriptorPool; }

		//FrameData& getFrameData(int currentFrame) { return frameData[currentFrame]; }
		VkDescriptorSet& getDescriptorSet(int currentFrame);

		void updateDescriptorSetTexture(VkImageView view,VkSampler sampler,
			uint32_t bindingSlot,int currentFrame);

	private:
		void createDescriptors();

		//FrameData frameData[MAX_FRAMES_IN_FLIGHT];

		VkDescriptorSetLayout globalSetLayout;
		VkDescriptorPool descriptorPool;

		std::vector<VkDescriptorSet> descriptorSets;

		Device& device;
		Buffer& buffer;
		Image& image;
	};

}
