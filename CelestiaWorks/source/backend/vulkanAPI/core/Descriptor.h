#pragma once
#include "backend/vulkanAPI/config/VulkanConfig.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <memory>

namespace celestia
{

	enum class DescriptorType
	{
		COMBINED_IMAGE_SAMPLER,
		STORAGE_IMAGE,
		BINDLESS_IMAGE,
		UNIFORM_BUFFER,
		STORAGE_BUFFER
	};

	struct Descriptor_t
	{
		VkDescriptorSet sets[MAX_FRAMES_IN_FLIGHT];
		VkDescriptorSetLayout layout;
	};

	struct BindingInfo
	{
		uint32_t binding;
		DescriptorType type;
		VkDescriptorType vkType;
		uint32_t descriptorCount;
		std::vector<VkDescriptorBufferInfo> bufferInfos;
		std::vector<VkDescriptorImageInfo> imageInfos;
	};

	class Descriptor
	{
	public:
		Descriptor();
		~Descriptor();

		// Add a binding to the descriptor set
		void addBinding(uint32_t binding, DescriptorType type, VkShaderStageFlags shaderStages, uint32_t descriptorCount = 1);

		// Build the descriptor set layout and allocate descriptor sets
		void build();

		// Update texture binding for a specific frame
		void updateTexture(uint32_t binding, const VkImageView* imageViews, VkSampler sampler, uint32_t count, uint32_t frameIndex = 0);

		// Update buffer binding for a specific frame
		void updateBuffer(uint32_t binding, VkBuffer* buffers, VkDeviceSize size, uint32_t count, uint32_t frameIndex = 0);

		// Apply all pending writes to the descriptor sets
		void flushWrites();

		// Clear all pending writes
		void clearWrites();

		// Getters
		VkDescriptorSet getDescriptorSet(uint32_t frameIndex = 0) const;
		VkDescriptorSetLayout getLayout() const;

	private:
		VkDescriptorType toVkType(DescriptorType type);
		BindingInfo* findBinding(uint32_t binding);
		void addWrite(uint32_t binding, VkDescriptorSet descriptorSet);

	private:
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
		std::unordered_map<uint32_t, BindingInfo> bindings;
		std::vector<VkWriteDescriptorSet> pendingWrites;

		std::vector<VkDescriptorPoolSize> poolSizes;
		VkDescriptorPool descriptorPool;

		// Descriptor set layout and sets
		VkDescriptorSetLayout descriptorLayout;
		VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];

		bool enableBindless;
		uint32_t bindingCount;
		bool isBuilt;
	};


}
