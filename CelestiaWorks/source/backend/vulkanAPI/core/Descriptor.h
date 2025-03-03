#pragma once
#include "backend/vulkanAPI/config/VulkanConfig.h"

#include <vulkan/vulkan.h>
#include <vector>

constexpr int MAX_WRITES = 2;
constexpr int TEXTURE_INDEX = 0;
constexpr int UNIFORM_BUFFER_INDEX = 1;


namespace celestia
{

	enum class DescriptorType
	{
		IMAGE,
		BINDLESS_IMAGE,
		UNIFORM_BUFFER,
		STORAGE_BUFFER
	};

	struct Descriptor_t
	{
		VkDescriptorSet sets[MAX_FRAMES_IN_FLIGHT];
		VkDescriptorSetLayout layout;
	};


	

	class DescriptorFactory
	{
	public:
		/*
		Mit‰ tarvii t‰‰ functio:
		Image classista: Tekstuurin imageView, textureSampler.
		Buffer classista: kyseinen bufferi / lista buffereita. esim uniform buffer

		option siihen ett‰ onko IMAGE bindless vai normaali. Eli n m‰‰r‰ tekstuureita shaderiin.
		Joku hassu tapa p‰ivitt‰‰ niiden arvoja jos tarvitsee.

		void addBinding(index, type, shader_stage)
		*/
		
		DescriptorFactory();
		void addBinding(uint32_t binding, DescriptorType type, VkShaderStageFlagBits shader,uint32_t descriptorCount = 1, uint32_t bufferInfoSize = 1);
		void build(VkDescriptorSet* descriptorSet, VkDescriptorSetLayout& descriptorLayout);


		void updateTexture(const VkImageView* view, const VkSampler sampler,
			const int bindingSlot, const int count, VkDescriptorSet set);

		void updateBuffer(VkBuffer* buffer, const VkDeviceSize size, const int bindingSlot, const int count, VkDescriptorSet set);
		void updateSets();

	private:
		VkDescriptorType toVkType(DescriptorType type);

	private:
		std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;
		uint32_t bindingCount;
		bool enableBindless; 

		std::vector<VkDescriptorPoolSize> poolSizes;
		VkDescriptorPool descriptorPool;

		std::vector<VkWriteDescriptorSet> writes;

		//uint32_t bufferCount;
		//uint32_t imageCount;

		std::vector<VkDescriptorBufferInfo> bufferInfo;
		std::vector<VkDescriptorImageInfo> imageInfo;

		//VkDescriptorSetLayoutBindingFlagsCreateInfo bindlesInfo; // TODO: tee t‰‰ loppuun.
	};


}
