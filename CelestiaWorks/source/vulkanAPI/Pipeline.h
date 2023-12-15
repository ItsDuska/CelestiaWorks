#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "celestiaTypes/Matrix.h"
#include "CelestiaVulkanTypes.h"

namespace celestia 
{
	class SwapChain;
	class Device;
	class ShaderObject;
	class Descriptor;


	enum class DrawingMode
	{
		TRIANGLE,
		POINTS,
		LINES
	};


	struct PUSH_CONSTANTS
	{
		Mat4 transform;
		Mat4 projection;
	};


	class BuildPipeline
	{
	public:
		VkPipeline buildPipeline(VkDevice device, VkRenderPass pass);

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages; // shaderit
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
		VkPipelineLayout pipelineLayout; // t rkee
	};

	class Pipeline
	{
	public:
		Pipeline(Device& device, SwapChain& swapChain,Descriptor& descriptor);
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator = (const Pipeline&) = delete;
		~Pipeline();

		void createPipeline(Material& material, ShaderObject &shader, DrawingMode drawMode, Descriptor* descriptors);
		Material *getDefaultMaterial();
	private:
		Material defaultMaterial;

		BuildPipeline builder;

		Device& device;
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
