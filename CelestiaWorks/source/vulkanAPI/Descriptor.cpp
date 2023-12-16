#include "Descriptor.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"
#include <array>

#define NUMBER_OF_TEXTURE_IN_SHADER 2

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

void celestia::Descriptor::updateDescriptorSetTexture(VkImageView view, VkSampler sampler,
	uint32_t bindingSlot, int currentFrame)
{
	std::array<VkDescriptorImageInfo, 2> imageInfo{};
	imageInfo[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo[0].imageView = view;
	imageInfo[0].sampler = sampler;

	imageInfo[1].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo[1].imageView = image.tempTexture.imageView;
	imageInfo[1].sampler = sampler;

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = buffer.uniformBufferTemp[currentFrame].buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	VkWriteDescriptorSet setWrite[2]{};
	setWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	setWrite[0].pNext = nullptr;
	setWrite[0].dstBinding = bindingSlot;
	setWrite[0].dstSet = getDescriptorSet(currentFrame);
	setWrite[0].descriptorCount = NUMBER_OF_TEXTURE_IN_SHADER;
	setWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	setWrite[0].pImageInfo = imageInfo.data();

	setWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	setWrite[1].pNext = nullptr;
	setWrite[1].dstBinding = 1;
	setWrite[1].dstSet = getDescriptorSet(currentFrame);
	setWrite[1].descriptorCount = 1;
	setWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	setWrite[1].pBufferInfo = &bufferInfo;


	vkUpdateDescriptorSets(device.device, 2, setWrite, 0, nullptr);
}

void celestia::Descriptor::createDescriptors()
{
	VkDescriptorSetLayoutBinding descriptorBufferBinding[2]{};
	descriptorBufferBinding[0].binding = 0;
	descriptorBufferBinding[0].descriptorCount = NUMBER_OF_TEXTURE_IN_SHADER; // testiä varten max 2 tekstuuria per draw call
	descriptorBufferBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //image sampler type.
	descriptorBufferBinding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //samper will be used in Fragment shader.

	descriptorBufferBinding[1].binding = 1;
	descriptorBufferBinding[1].descriptorCount = 1; 
	descriptorBufferBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
	descriptorBufferBinding[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;
	setinfo.bindingCount = 2;
	setinfo.flags = 0;
	setinfo.pBindings = descriptorBufferBinding;

	vkCreateDescriptorSetLayout(device.device, &setinfo, nullptr, &globalSetLayout);

	std::vector<VkDescriptorPoolSize> poolSizes =
	{
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * NUMBER_OF_TEXTURE_IN_SHADER) },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT ) }
	};

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = 0;
	poolInfo.maxSets = 4+2; //static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();

	vkCreateDescriptorPool(device.device, &poolInfo, nullptr, &descriptorPool);

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

		updateDescriptorSetTexture(image.defaultTexture.imageView, image.textureSampler, 0, i);
	}
}
