#pragma once
#include "vulkan/Buffer.h"

namespace celestia {

	class Descriptor
	{
	public:
		Descriptor(Device& deive, Buffer& buffer);
		~Descriptor();

		VkDescriptorSetLayout& getDescriptorSetLayout() { return globalSetLayout; }
		VkDescriptorPool& getDescriptorPool() { return descriptorPool; }

		FrameData& getFrameData(int currentFrame) { return frameData[currentFrame]; }
	private:
		void createDescriptors();

		FrameData frameData[MAX_FRAMES_IN_FLIGHT];

		VkDescriptorSetLayout globalSetLayout;
		VkDescriptorPool descriptorPool;


		Device& device;
		Buffer& buffer;
	};

}

