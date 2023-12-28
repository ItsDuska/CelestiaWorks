#include "Descriptor.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"
#include <array>


celestia::Descriptor::Descriptor(Buffer& buffer,Image& image)
	: buffer{buffer}, image{image}
{
	writes.resize(MAX_WRITES);
	createDescriptors();
}

celestia::Descriptor::~Descriptor()
{
}

VkDescriptorSet& celestia::Descriptor::getDescriptorSet(int currentFrame)
{
	return descriptorSets[currentFrame];
}

void celestia::Descriptor::updateDescriptorSetTextures(const VkImageView* view, const VkSampler sampler,
	const int bindingSlot, const int count, const int currentFrame)
{
	for (int i = 0; i < count; i++)
	{
		imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (view == nullptr)
		{
			imageInfo[i].imageView = image.defaultTexture.imageView;
		}
		else
		{
			imageInfo[i].imageView = view[i];
		}

		imageInfo[i].sampler = sampler;
	}

	writes[TEXTURE_INDEX].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[TEXTURE_INDEX].pNext = nullptr;
	writes[TEXTURE_INDEX].dstBinding = bindingSlot;
	writes[TEXTURE_INDEX].dstSet = getDescriptorSet(currentFrame);
	writes[TEXTURE_INDEX].descriptorCount = count;
	writes[TEXTURE_INDEX].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[TEXTURE_INDEX].pImageInfo = imageInfo;
}

void celestia::Descriptor::uppdateDescriporSetUniformBuffers(const VkBuffer* buffer, const VkDeviceSize size,
	const int bindingSlot, const int count, const int currentFrame)
{
	for (int i = 0; i<count; i++)
	{
		bufferInfo[i].buffer = buffer[i];
		bufferInfo[i].offset = 0;
		bufferInfo[i].range = size;
	}

	writes[UNIFORM_BUFFER_INDEX].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[UNIFORM_BUFFER_INDEX].pNext = nullptr;
	writes[UNIFORM_BUFFER_INDEX].dstBinding = bindingSlot;
	writes[UNIFORM_BUFFER_INDEX].dstSet = getDescriptorSet(currentFrame);
	writes[UNIFORM_BUFFER_INDEX].descriptorCount = count;
	writes[UNIFORM_BUFFER_INDEX].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[UNIFORM_BUFFER_INDEX].pBufferInfo = bufferInfo;	
}

void celestia::Descriptor::updateAllDescriptorSets()
{
	vkUpdateDescriptorSets(Device::context.device, 2, writes.data(), 0, nullptr);
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

	VkDescriptorBindingFlags flags[2]{};
	flags[0] = 0;
	flags[1] = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;

	VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlags{};
	bindingFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	bindingFlags.bindingCount = 2;
	bindingFlags.pBindingFlags = flags;

	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = &bindingFlags;
	setinfo.bindingCount = 2;
	setinfo.flags = 0;
	setinfo.pBindings = descriptorBufferBinding;

	vkCreateDescriptorSetLayout(Device::context.device, &setinfo, nullptr, &globalSetLayout);

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

	vkCreateDescriptorPool(Device::context.device, &poolInfo, nullptr, &descriptorPool);

	Device::context.deletionQueue.pushFunction([&]() {
		vkDestroyDescriptorSetLayout(Device::context.device, globalSetLayout, nullptr);
		vkDestroyDescriptorPool(Device::context.device, descriptorPool, nullptr);
		});

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		uint32_t counts[1] = {};
		counts[0] = 1;
		VkDescriptorSetVariableDescriptorCountAllocateInfo setCounts = {};
		setCounts.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
		setCounts.descriptorSetCount = 1;
		setCounts.pDescriptorCounts = counts;

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = &setCounts;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &globalSetLayout;
		vkAllocateDescriptorSets(Device::context.device, &allocInfo, &descriptorSets[i]);

		updateDescriptorSetTextures(nullptr, image.textureSampler, 0, NUMBER_OF_TEXTURE_IN_SHADER, i);
		uppdateDescriporSetUniformBuffers(&buffer.uniformBufferTemp[i].buffer, sizeof(UniformBufferObject), 1, NUMBER_OF_UNIFORM_BUFFERS, i);
		updateAllDescriptorSets();
	}
}
