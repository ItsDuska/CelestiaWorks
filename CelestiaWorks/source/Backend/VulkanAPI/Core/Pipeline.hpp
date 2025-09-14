#pragma once
#include "CelestiaVulkanTypes.hpp"
#include "Backend/Utils/Utils.hpp"

namespace celestia
{
	class SwapChain;
	class ShaderObject;
	class Descriptor;

	enum class DrawingMode
	{
		TRIANGLE,
		POINTS,
		LINES
	};

	class BuildPipeline
	{
	public:
		VkPipeline buildPipeline(VkDevice device, VkRenderPass pass);

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		VkPipelineViewportStateCreateInfo viewportState;
		VkPipelineVertexInputStateCreateInfo vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssembly;
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineDepthStencilStateCreateInfo depthStencil;
		// VkPipelineDynamicStateCreateInfo dynamicInfo;
		VkPipelineRasterizationStateCreateInfo rasterizer;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo multisampling;
		VkPipelineLayout pipelineLayout;
	};

	struct PipelineOptions
	{
		bool blending;
	};

	class Pipeline
	{
	public:
		Pipeline() = default;
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;
		~Pipeline() = default;

		const Material createPipeline(
		  ShaderObject& shader, DrawingMode drawMode, VkDescriptorSetLayout* descriptors, VkRenderPass renderpass);
		// Material *getDefaultMaterial();
	private:
		// Material defaultMaterial;

		BuildPipeline builder;

	public:
		void createInputAssembly(DrawingMode mode);
		void createViewport(Vec2 position, Vec2 dimensions);
		void createScissors(Vec2i offset, VkExtent2D extent);
		// VkPipelineDynamicStateCreateInfo createDynamicState();
		// VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp
		// compareOp);
		void createRasterizer(DrawingMode mode);
		void createMultisampling();
		void createColorBlendAttachment(bool blending);
		void
		createVertexInputStateCreateInfo(utils::CustomVertexInputAttributeDescriptionFactory& attributeDescriptions,
		  const VkVertexInputBindingDescription& bindingDescription, uint32_t count);

	private:
		VkPipelineLayoutCreateInfo createLayoutInfo(ShaderObject& shader, VkDescriptorSetLayout* descriptors);
	};

} // namespace celestia
