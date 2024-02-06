#pragma once
#include "CelestiaVulkanTypes.h"

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
		//VkPipelineDynamicStateCreateInfo dynamicInfo;
		VkPipelineRasterizationStateCreateInfo rasterizer;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo multisampling;
		VkPipelineLayout pipelineLayout;
	};

	class Pipeline
	{
	public:
		Pipeline(SwapChain& swapChain,Descriptor& descriptor);
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator = (const Pipeline&) = delete;
		~Pipeline();

		void createPipeline(Material& material, ShaderObject &shader, DrawingMode drawMode, Descriptor* descriptors);
		Material *getDefaultMaterial();
	private:
		Material defaultMaterial;

		BuildPipeline builder;

		SwapChain& swapChain;
		Descriptor& descriptor;
	private:
		VkPipelineInputAssemblyStateCreateInfo createInputAssembly(DrawingMode mode);
		VkViewport createViewport();
		VkRect2D createScissors();
		//VkPipelineDynamicStateCreateInfo createDynamicState();
		//VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
		VkPipelineRasterizationStateCreateInfo createRasterizer(DrawingMode mode);
		VkPipelineMultisampleStateCreateInfo createMultisampling();
		VkPipelineColorBlendAttachmentState createColorBlendAttachment();
		VkPipelineLayoutCreateInfo createLayoutInfo(ShaderObject& shader, Descriptor* descriptor);
	};

}
