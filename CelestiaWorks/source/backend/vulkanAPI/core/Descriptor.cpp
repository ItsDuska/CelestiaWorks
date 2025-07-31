#include "Descriptor.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"
#include <array>
#include <iostream>

celestia::Descriptor::Descriptor()
{
    bindingCount = 0;
    enableBindless = false;
    descriptorPool = VK_NULL_HANDLE;
    descriptorLayout = VK_NULL_HANDLE;
    isBuilt = false;

    // Initialize descriptor sets to VK_NULL_HANDLE
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        descriptorSets[i] = VK_NULL_HANDLE;
    }
}

celestia::Descriptor::~Descriptor()
{
    // Cleanup is handled by the deletion queue in Device class
    // This is called when the device is destroyed
}

void celestia::Descriptor::addBinding(uint32_t binding, DescriptorType type, VkShaderStageFlags shaderStages, uint32_t descriptorCount)
{
    VkDescriptorType vkType = toVkType(type);

    // Check if this is a bindless texture array
    if (type == DescriptorType::BINDLESS_IMAGE)
    {
        enableBindless = true;
        descriptorCount = NUMBER_OF_TEXTURE_IN_SHADER; // Ensure descriptor count matches allocation
    }

    // Create binding info structure
    BindingInfo bindingInfo;
    bindingInfo.binding = binding;
    bindingInfo.type = type;
    bindingInfo.vkType = vkType;
    bindingInfo.descriptorCount = descriptorCount;

    // Pre-allocate info arrays based on type
    if (type == DescriptorType::COMBINED_IMAGE_SAMPLER || type == DescriptorType::STORAGE_IMAGE || type == DescriptorType::BINDLESS_IMAGE)
    {
        bindingInfo.imageInfos.resize(descriptorCount);
    }
    else
    {
        bindingInfo.bufferInfos.resize(descriptorCount);
    }

    // Store binding info
    bindings[binding] = std::move(bindingInfo);

    // Create layout binding
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = descriptorCount;
    layoutBinding.descriptorType = vkType;
    layoutBinding.stageFlags = shaderStages;
    layoutBinding.pImmutableSamplers = nullptr;

    layoutBindings.push_back(layoutBinding);

    // Add to pool sizes
    poolSizes.push_back({ vkType, MAX_FRAMES_IN_FLIGHT * descriptorCount });

    bindingCount++;
}

void celestia::Descriptor::build()
{
    if (isBuilt)
    {
        std::cerr << "Warning: Descriptor already built!" << std::endl;
        return;
    }

    // Setup binding flags for bindless support
    std::vector<VkDescriptorBindingFlags> bindingFlags;
    if (enableBindless)
    {
        bindingFlags.resize(layoutBindings.size(), VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT);
        // Make the last binding variable count if bindless
        if (!bindingFlags.empty()) {
            bindingFlags.back() |= VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;
        }
    }

    VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsCreateInfo{};
    bindingFlagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    bindingFlagsCreateInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
    bindingFlagsCreateInfo.pBindingFlags = bindingFlags.data();

    // Create descriptor set layout
    VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.pNext = enableBindless ? &bindingFlagsCreateInfo : nullptr;
    layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    layoutCreateInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(Device::context.device, &layoutCreateInfo, nullptr, &descriptorLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }

    // Create descriptor pool
    VkDescriptorPoolCreateInfo poolCreateInfo{};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.flags = 0;
    poolCreateInfo.maxSets = MAX_FRAMES_IN_FLIGHT;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.data();

    if (vkCreateDescriptorPool(Device::context.device, &poolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool!");
    }

    // Add cleanup to deletion queue
    Device::context.deletionQueue.pushFunction([=]() {
        vkDestroyDescriptorSetLayout(Device::context.device, descriptorLayout, nullptr);
        vkDestroyDescriptorPool(Device::context.device, descriptorPool, nullptr);
        });

    // Allocate descriptor sets
    uint32_t variableDescriptorCount = NUMBER_OF_TEXTURE_IN_SHADER;
    VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo{};
    variableCountInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    variableCountInfo.descriptorSetCount = 1;
    variableCountInfo.pDescriptorCounts = &variableDescriptorCount;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorSetAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo.pNext = enableBindless ? &variableCountInfo : nullptr;
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &descriptorLayout;

        if (vkAllocateDescriptorSets(Device::context.device, &allocateInfo, &descriptorSets[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate descriptor sets!");
        }
    }

    isBuilt = true;
}

void celestia::Descriptor::updateTexture(uint32_t binding, const VkImageView* imageViews, VkSampler sampler, uint32_t count, uint32_t frameIndex)
{
    if (!isBuilt)
    {
        std::cerr << "Warning: Descriptor not built yet!" << std::endl;
        return;
    }

    if (frameIndex >= MAX_FRAMES_IN_FLIGHT)
    {
        std::cerr << "Warning: Frame index " << frameIndex << " is out of range!" << std::endl;
        return;
    }

    BindingInfo* bindingInfo = findBinding(binding);
    if (!bindingInfo)
    {
        std::cerr << "Warning: Binding " << binding << " not found!" << std::endl;
        return;
    }

    if (bindingInfo->type != DescriptorType::COMBINED_IMAGE_SAMPLER &&
        bindingInfo->type != DescriptorType::STORAGE_IMAGE &&
        bindingInfo->type != DescriptorType::BINDLESS_IMAGE)
    {
        std::cerr << "Warning: Binding " << binding << " is not an image binding!" << std::endl;
        return;
    }

    // Ensure we don't exceed the allocated size
    count = std::min(count, static_cast<uint32_t>(bindingInfo->imageInfos.size()));

    // Update image infos
    for (uint32_t i = 0; i < count; i++)
    {
        // Set appropriate image layout based on descriptor type
        if (bindingInfo->type == DescriptorType::STORAGE_IMAGE)
        {
            bindingInfo->imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            bindingInfo->imageInfos[i].sampler = VK_NULL_HANDLE; // Storage images don't use samplers
        }
        else
        {
            bindingInfo->imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            bindingInfo->imageInfos[i].sampler = sampler;
        }
        bindingInfo->imageInfos[i].imageView = (imageViews[i] != VK_NULL_HANDLE) ? imageViews[i] : imageViews[0]; // fallback to first image
    }

    // Create write descriptor set
    VkWriteDescriptorSet writeDescriptorSet{};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSets[frameIndex];
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = bindingInfo->vkType;
    writeDescriptorSet.descriptorCount = count;
    writeDescriptorSet.pImageInfo = bindingInfo->imageInfos.data();

    pendingWrites.push_back(writeDescriptorSet);
}

void celestia::Descriptor::updateBuffer(uint32_t binding, VkBuffer* buffers, VkDeviceSize size, uint32_t count, uint32_t frameIndex)
{
    if (!isBuilt)
    {
        std::cerr << "Warning: Descriptor not built yet!" << std::endl;
        return;
    }

    if (frameIndex >= MAX_FRAMES_IN_FLIGHT)
    {
        std::cerr << "Warning: Frame index " << frameIndex << " is out of range!" << std::endl;
        return;
    }

    BindingInfo* bindingInfo = findBinding(binding);
    if (!bindingInfo)
    {
        std::cerr << "Warning: Binding " << binding << " not found!" << std::endl;
        return;
    }

    if (bindingInfo->type != DescriptorType::UNIFORM_BUFFER && bindingInfo->type != DescriptorType::STORAGE_BUFFER)
    {
        std::cerr << "Warning: Binding " << binding << " is not a buffer binding!" << std::endl;
        return;
    }

    // Ensure we don't exceed the allocated size
    count = std::min(count, static_cast<uint32_t>(bindingInfo->bufferInfos.size()));

    // Update buffer infos
    for (uint32_t i = 0; i < count; i++)
    {
        bindingInfo->bufferInfos[i].buffer = buffers[i];
        bindingInfo->bufferInfos[i].offset = 0;
        bindingInfo->bufferInfos[i].range = size;
    }

    // Create write descriptor set
    VkWriteDescriptorSet writeDescriptorSet{};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSets[frameIndex];
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = bindingInfo->vkType;
    writeDescriptorSet.descriptorCount = count;
    writeDescriptorSet.pBufferInfo = bindingInfo->bufferInfos.data();

    pendingWrites.push_back(writeDescriptorSet);
}

void celestia::Descriptor::flushWrites()
{
    if (!pendingWrites.empty())
    {
        vkUpdateDescriptorSets(Device::context.device, static_cast<uint32_t>(pendingWrites.size()), pendingWrites.data(), 0, nullptr);
        pendingWrites.clear();
    }
}

void celestia::Descriptor::clearWrites()
{
    pendingWrites.clear();
}

VkDescriptorType celestia::Descriptor::toVkType(DescriptorType type)
{
    switch (type)
    {
    case DescriptorType::STORAGE_IMAGE:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case DescriptorType::BINDLESS_IMAGE:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case DescriptorType::UNIFORM_BUFFER:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::STORAGE_BUFFER:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case DescriptorType::COMBINED_IMAGE_SAMPLER:
    default:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    }
}

celestia::BindingInfo* celestia::Descriptor::findBinding(uint32_t binding)
{
    auto it = bindings.find(binding);
    return (it != bindings.end()) ? &it->second : nullptr;
}

void celestia::Descriptor::addWrite(uint32_t binding, VkDescriptorSet descriptorSet)
{
    BindingInfo* bindingInfo = findBinding(binding);
    if (!bindingInfo) return;

    VkWriteDescriptorSet writeDescriptorSet{};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = descriptorSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorType = bindingInfo->vkType;
    writeDescriptorSet.descriptorCount = bindingInfo->descriptorCount;

    if (bindingInfo->type == DescriptorType::COMBINED_IMAGE_SAMPLER ||
        bindingInfo->type == DescriptorType::STORAGE_IMAGE ||
        bindingInfo->type == DescriptorType::BINDLESS_IMAGE) {
        writeDescriptorSet.pImageInfo = bindingInfo->imageInfos.data();
    }
    else
    {
        writeDescriptorSet.pBufferInfo = bindingInfo->bufferInfos.data();
    }

    pendingWrites.push_back(writeDescriptorSet);
}

VkDescriptorSet celestia::Descriptor::getDescriptorSet(uint32_t frameIndex) const
{
    if (frameIndex >= MAX_FRAMES_IN_FLIGHT)
    {
        std::cerr << "Warning: Frame index " << frameIndex << " is out of range!" << std::endl;
        return VK_NULL_HANDLE;
    }

    if (!isBuilt)
    {
        std::cerr << "Warning: Descriptor not built yet!" << std::endl;
        return VK_NULL_HANDLE;
    }

    return descriptorSets[frameIndex];
}

VkDescriptorSetLayout celestia::Descriptor::getLayout() const
{
    if (!isBuilt)
    {
        std::cerr << "Warning: Descriptor not built yet!" << std::endl;
        return VK_NULL_HANDLE;
    }

    return descriptorLayout;
}
