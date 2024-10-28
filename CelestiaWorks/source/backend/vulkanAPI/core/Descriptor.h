#pragma once
#include "backend/vulkanAPI/config/VulkanConfig.h"

#include <vulkan/vulkan.h>
#include <vector>

constexpr int MAX_WRITES = 2;
constexpr int TEXTURE_INDEX = 0;
constexpr int UNIFORM_BUFFER_INDEX = 1;

/*
TODO:
KIRJOTA TÄÄ KAIKKI UUDESTAAN, JOTTA PYSTYISI LUOMAAN MITÄ TAHANSA HELPOSTI!
*/


namespace celestia
{
	class Image;

	class Descriptor
	{
	public:
		Descriptor(Image& image);
		~Descriptor();

		VkDescriptorSetLayout& getDescriptorSetLayout() { return globalSetLayout; }

		VkDescriptorPool& getDescriptorPool() { return descriptorPool; }

		VkDescriptorSet& getDescriptorSet(int currentFrame);
		VkDescriptorSet* getDefaultSpriteDescriptorSets();

		void updateDescriptorSetTextures(const VkImageView* view, const VkSampler sampler,
			const int bindingSlot, const int count, const int currentFrame);

		void uppdateDescriporSetUniformBuffers(const VkBuffer* buffer,const VkDeviceSize size,
			const int bindingSlot,const int count, const int currentFrame);

		void updateAllDescriptorSets();
	private:
		void createDescriptors();
	private:

		VkDescriptorSetLayout globalSetLayout;
		VkDescriptorPool descriptorPool;

		VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];

		VkDescriptorBufferInfo bufferInfo[NUMBER_OF_UNIFORM_BUFFERS];
		VkDescriptorImageInfo imageInfo[NUMBER_OF_TEXTURE_IN_SHADER];
		std::vector<VkWriteDescriptorSet> writes;

		Image& image;
	};


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
		Mitä tarvii tää functio:
		Image classista: Tekstuurin imageView, textureSampler.
		Buffer classista: kyseinen bufferi / lista buffereita. esim uniform buffer

		option siihen että onko IMAGE bindless vai normaali. Eli n määrä tekstuureita shaderiin.
		Joku hassu tapa päivittää niiden arvoja jos tarvitsee.

		void addBinding(index, type, shader_stage)
		*/
		
		DescriptorFactory();
		void createDescriptor();
		void addBinding(uint32_t binding, DescriptorType type, VkShaderStageFlagBits shader,uint32_t descriptorCount = 1);
		void build(VkDescriptorSet* descriptorSet, VkDescriptorSetLayout& descriptorLayout);


		void updateTexture(const VkImageView& view, VkSampler& sampler, VkDescriptorSet& descriptorSet);
		void updateBuffer(VkBuffer& buffer, const VkDeviceSize size, VkDescriptorSet& descriptorSet);
		void updateSets();

	private:
		VkDescriptorType toVkType(DescriptorType type);

	private:
		std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;
		uint32_t bindingCount;

		std::vector<VkDescriptorPoolSize> poolSizes;
		VkDescriptorPool descriptorPool;

		std::vector<VkWriteDescriptorSet> writes;

		//uint32_t bufferCount;
		//uint32_t imageCount;

		VkDescriptorBufferInfo bufferInfo;
		VkDescriptorImageInfo imageInfo;

		//VkDescriptorSetLayoutBindingFlagsCreateInfo bindlesInfo; // TODO: tee tää loppuun.
	};


}
