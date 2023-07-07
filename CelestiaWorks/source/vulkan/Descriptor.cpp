#include "Descriptor.h"

celestia::Descriptor::Descriptor(Device& device, Buffer& buffer)
	:device{device}, buffer{buffer}
{
	createDescriptors();
}

celestia::Descriptor::~Descriptor()
{
}

void celestia::Descriptor::createDescriptors()
{
	VkDescriptorSetLayoutBinding camBufferBinding = {};
	camBufferBinding.binding = 0;
	camBufferBinding.descriptorCount = 1;
	// it's a uniform buffer binding
	camBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	// we use it from the vertex shader
	camBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;

	//we are going to have 1 binding
	setinfo.bindingCount = 1;
	//no flags
	setinfo.flags = 0;
	//point to the camera buffer binding
	setinfo.pBindings = &camBufferBinding;

	vkCreateDescriptorSetLayout(device.getDevice(), &setinfo, nullptr, &globalSetLayout);

	std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(device.getDevice(), &pool_info, nullptr, &descriptorPool);

	device.deletionQueue.pushFunction([&]() {
		vkDestroyDescriptorSetLayout(device.getDevice(), globalSetLayout, nullptr);
		vkDestroyDescriptorPool(device.getDevice(), descriptorPool, nullptr);
		});


	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		buffer.createBuffer(sizeof(CameraData),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			frameData[i].cameraBuffer.buffer, frameData[i].cameraBuffer.bufferMemory
		);

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		//using the pool we just set
		allocInfo.descriptorPool = descriptorPool;
		//only 1 descriptor
		allocInfo.descriptorSetCount = 1;
		//using the global data layout
		allocInfo.pSetLayouts = &globalSetLayout;

		vkAllocateDescriptorSets(device.getDevice(), &allocInfo, &frameData[i].globalDescriptor);

		VkDescriptorBufferInfo binfo;
		//it will be the camera buffer
		binfo.buffer = frameData[i].cameraBuffer.buffer;
		//at 0 offset
		binfo.offset = 0;
		//of the size of a camera data struct
		binfo.range = sizeof(CameraData);

		VkWriteDescriptorSet setWrite = {};
		setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		setWrite.pNext = nullptr;

		//we are going to write into binding number 0
		setWrite.dstBinding = 0;
		//of the global descriptor
		setWrite.dstSet = frameData[i].globalDescriptor;

		setWrite.descriptorCount = 1;
		//and the type is uniform buffer
		setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		setWrite.pBufferInfo = &binfo;


		vkUpdateDescriptorSets(device.getDevice(), 1, &setWrite, 0, nullptr);

	}

	// add buffers to deletion queues
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		device.deletionQueue.pushFunction([=]() {
			vkDestroyBuffer(device.getDevice(), frameData[i].cameraBuffer.buffer, nullptr);
			vkFreeMemory(device.getDevice(), frameData[i].cameraBuffer.bufferMemory, nullptr);
			});
	}
}
