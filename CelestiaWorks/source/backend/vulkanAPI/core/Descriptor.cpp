#include "Descriptor.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"
#include <array>



celestia::Descriptor::Descriptor()
{
    bindingCount = 0;
    enableBindless = false;
    descriptorPool = VK_NULL_HANDLE;
}

void celestia::Descriptor::addBinding(uint32_t binding, DescriptorType type,
    VkShaderStageFlagBits shader, uint32_t descriptorCount, uint32_t bufferInfoSize)
{
    VkDescriptorType vkType = toVkType(type);
    if (type == DescriptorType::IMAGE || type == DescriptorType::BINDLESS_IMAGE)
    {
        imageInfo.resize(bufferInfoSize);
    }
    else
    {
        bufferInfo.resize(bufferInfoSize);
    }

    if (type == DescriptorType::BINDLESS_IMAGE) {
        enableBindless = true;
        descriptorCount = NUMBER_OF_TEXTURE_IN_SHADER; // Ensure descriptor count matches allocation
    }

    VkDescriptorSetLayoutBinding layoutbinding{};
    layoutbinding.binding = bindingCount;
    layoutbinding.descriptorCount = descriptorCount;
    layoutbinding.descriptorType = vkType;
    layoutbinding.stageFlags = shader;
    descriptorBindings.push_back(layoutbinding);
    poolSizes.push_back({ vkType,MAX_FRAMES_IN_FLIGHT * descriptorCount });

    bindingCount++;
}

void celestia::Descriptor::build(VkDescriptorSet* descriptorSet, VkDescriptorSetLayout& descriptorLayout)
{   
    std::vector<VkDescriptorBindingFlags> flags(bindingCount, VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT);

    if (bindingCount > 0)
    {
        flags[bindingCount - 1] |= VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;
    }

    VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlags{};
    bindingFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    bindingFlags.bindingCount = bindingCount;
    bindingFlags.pBindingFlags = flags.data();

    void* bindlessFlagPtr = nullptr;
    if (enableBindless)
    {
        bindlessFlagPtr = &bindingFlags;
    }

    VkDescriptorSetLayoutCreateInfo setinfo = {};
    setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setinfo.pNext = bindlessFlagPtr;
    setinfo.bindingCount = bindingCount;
    setinfo.flags = 0;
    setinfo.pBindings = descriptorBindings.data();

    vkCreateDescriptorSetLayout(Device::context.device, &setinfo, nullptr, &descriptorLayout);

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = 0;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();

    vkCreateDescriptorPool(Device::context.device, &poolInfo, nullptr, &descriptorPool);

    Device::context.deletionQueue.pushFunction([&]() {
        vkDestroyDescriptorSetLayout(Device::context.device, descriptorLayout, nullptr);
        vkDestroyDescriptorPool(Device::context.device, descriptorPool, nullptr);
    });

    writes.resize(this->bindingCount);

    uint32_t counts[1] = { NUMBER_OF_TEXTURE_IN_SHADER };
    VkDescriptorSetVariableDescriptorCountAllocateInfo setCounts = {};
    setCounts.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    setCounts.descriptorSetCount = 1;
    setCounts.pDescriptorCounts = counts;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        void* countPtr = nullptr;
        if (enableBindless)
        {
            countPtr = &setCounts;
        }

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.pNext = countPtr;
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorLayout;

        if (vkAllocateDescriptorSets(Device::context.device, &allocInfo, &descriptorSet[i]) != VK_SUCCESS)
        {
            std::cerr << "ALLOCATION ERROR!\n";
        }
    }
}

void celestia::Descriptor::updateTexture(const VkImageView* view, const VkSampler sampler,
    const int bindingSlot, const int count, VkDescriptorSet set)
{
    for (int i = 0; i < count; i++)
    {
        imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        if (view[i] == nullptr)
        {
            imageInfo[i].imageView = view[0]; // default texture
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
    writes[TEXTURE_INDEX].dstSet = set;
    writes[TEXTURE_INDEX].descriptorCount = count;
    writes[TEXTURE_INDEX].dstArrayElement = 0;
    writes[TEXTURE_INDEX].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[TEXTURE_INDEX].pImageInfo = imageInfo.data();
}

void celestia::Descriptor::updateBuffer(VkBuffer* buffer, const VkDeviceSize size, const int bindingSlot, const int count, VkDescriptorSet set)
{
    for (int i = 0; i < count; i++)
    {
        bufferInfo[i].buffer = buffer[i];
        bufferInfo[i].offset = 0;
        bufferInfo[i].range = size;
    }

    writes[UNIFORM_BUFFER_INDEX].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[UNIFORM_BUFFER_INDEX].pNext = nullptr;
    writes[UNIFORM_BUFFER_INDEX].dstBinding = bindingSlot;
    writes[UNIFORM_BUFFER_INDEX].dstSet = set;
    writes[UNIFORM_BUFFER_INDEX].descriptorCount = count;
    writes[UNIFORM_BUFFER_INDEX].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[UNIFORM_BUFFER_INDEX].pBufferInfo = bufferInfo.data();
}

void celestia::Descriptor::updateSets()
{
    vkUpdateDescriptorSets(Device::context.device, bindingCount, writes.data(), 0, nullptr);
}

VkDescriptorType celestia::Descriptor::toVkType(DescriptorType type)
{
    switch (type)
    {
    case celestia::DescriptorType::IMAGE:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case celestia::DescriptorType::BINDLESS_IMAGE:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        // tee t�ss� jotain erikoista
    case celestia::DescriptorType::UNIFORM_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case celestia::DescriptorType::STORAGE_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; //TODO: en tii� pit��k� olla dynamic!
    default:
        break;
    }
    return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
}
