#include "Descriptor.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"

celestia::Descriptor::Descriptor(Device& device, Buffer& buffer,Image& image)
	:device{device},buffer{buffer}, image{image}
{
	createDescriptors();
}

celestia::Descriptor::~Descriptor()
{
}

VkDescriptorSet& celestia::Descriptor::getDescriptorSet(int currentFrame)
{
	return descriptorSets[currentFrame];
}

void celestia::Descriptor::createDescriptors()
{
	VkDescriptorSetLayoutBinding descriptorBufferBinding = {};
	descriptorBufferBinding.binding = 0;
	descriptorBufferBinding.descriptorCount = 1;
	descriptorBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //image sampler type.
	descriptorBufferBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //samper will be used in Fragment shader.

	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;
	setinfo.bindingCount = 1;
	setinfo.flags = 0;
	setinfo.pBindings = &descriptorBufferBinding;

	vkCreateDescriptorSetLayout(device.device, &setinfo, nullptr, &globalSetLayout);

	std::vector<VkDescriptorPoolSize> poolSizes =
	{
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT) }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	pool_info.poolSizeCount = (uint32_t)poolSizes.size();
	pool_info.pPoolSizes = poolSizes.data();

	vkCreateDescriptorPool(device.device, &pool_info, nullptr, &descriptorPool);

	device.deletionQueue.pushFunction([&]() {
		vkDestroyDescriptorSetLayout(device.device, globalSetLayout, nullptr);
		vkDestroyDescriptorPool(device.device, descriptorPool, nullptr);
		});

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &globalSetLayout;
		vkAllocateDescriptorSets(device.device, &allocInfo, &descriptorSets[i]);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = image.textureImageView;
		imageInfo.sampler = image.textureSampler;

		VkWriteDescriptorSet setWrite = {};
		setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		setWrite.pNext = nullptr;
		setWrite.dstBinding = 0;
		setWrite.dstSet = descriptorSets[i];
		setWrite.descriptorCount = 1;
		setWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		setWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device.device, 1, &setWrite, 0, nullptr);
	}
}
