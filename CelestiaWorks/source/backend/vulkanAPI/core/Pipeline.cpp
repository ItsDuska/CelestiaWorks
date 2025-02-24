#include "Pipeline.h"
#include "Device.h"
#include "SwapChain.h"
#include "ShaderObject.h"
#include "Descriptor.h"
#include "backend/utils/Utils.h"


// TODO: Tee t‰st‰ template functio. template <typename Vertex_t> 
// T‰m‰n avulla voidaan m‰‰ritell‰ custom vertex type.
// 
//Use nullptr for descriptor if not using any uniform buffers or textures.
// T‰st‰ pit‰‰ tulla myˆs funktio jota voidaan k‰ytt‰‰ kaikkialla muualla

const celestia::Material celestia::Pipeline::createPipeline(ShaderObject& shader,
	DrawingMode drawMode,
	VkDescriptorSetLayout* descriptors,
	VkRenderPass renderpass)
{
	Material material{};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = createLayoutInfo(shader, descriptors);

	if (vkCreatePipelineLayout(Device::context.device, &pipelineLayoutInfo, nullptr, &material.layout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}

	builder.shaderStages = shader.getInfos();
	builder.pipelineLayout = material.layout;

	material.pipeline = builder.buildPipeline(Device::context.device, renderpass);


	Device::context.deletionQueue.pushFunction([=]() {
		vkDestroyPipeline(Device::context.device, material.pipeline, nullptr);
		vkDestroyPipelineLayout(Device::context.device, material.layout, nullptr);
		}
	);
	return material;
}

void celestia::Pipeline::createInputAssembly(DrawingMode mode)
{
	VkPipelineInputAssemblyStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.primitiveRestartEnable = VK_FALSE;

	switch (mode)
	{
	case celestia::DrawingMode::TRIANGLE:
		info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		break;
	case celestia::DrawingMode::POINTS:
		info.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		break;
	case celestia::DrawingMode::LINES:
		info.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		break;
	default:
		break;
	}

	builder.inputAssembly = info;
}

void celestia::Pipeline::createViewport(Vec2 position, Vec2 dimensions)
{
	VkViewport result{};
	result.x = position.x;
	result.y = position.y;
	result.width = dimensions.x;
	result.height = dimensions.y;

	result.minDepth = 0.0f;
	result.maxDepth = 1.0f;

	builder.viewport = result;
}

void celestia::Pipeline::createScissors(Vec2i offset, VkExtent2D extent)
{
	VkRect2D scissor{};
	scissor.offset = { offset.x, offset.y };
	scissor.extent = extent;

	builder.scissor = scissor;
}

void celestia::Pipeline::createRasterizer(DrawingMode mode)
{
	VkPipelineRasterizationStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.depthClampEnable = VK_FALSE;
	info.rasterizerDiscardEnable = VK_FALSE;
	info.lineWidth = 1.0f;
	info.cullMode = VK_CULL_MODE_NONE;
	info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	info.depthBiasEnable = VK_FALSE;
	info.depthBiasConstantFactor = 0.0f;
	info.depthBiasClamp = 0.0f;
	info.depthBiasSlopeFactor = 0.0f;

	switch (mode)
	{
	case celestia::DrawingMode::TRIANGLE:
		info.polygonMode = VK_POLYGON_MODE_FILL;
		break;
	case celestia::DrawingMode::POINTS:
		info.polygonMode = VK_POLYGON_MODE_POINT;
		break;
	case celestia::DrawingMode::LINES:
		info.polygonMode = VK_POLYGON_MODE_LINE;
		break;
	default:
		break;
	}

	builder.rasterizer = info;
}

void celestia::Pipeline::createMultisampling()
{
	VkPipelineMultisampleStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.sampleShadingEnable = VK_FALSE;
	info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	info.minSampleShading = 1.0f;
	info.pSampleMask = nullptr;
	info.alphaToCoverageEnable = VK_FALSE;
	info.alphaToOneEnable = VK_FALSE;

	builder.multisampling = info;
}

void celestia::Pipeline::createColorBlendAttachment(bool blending)
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = blending;

	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; //VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; //VK_BLEND_FACTOR_DST_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	builder.colorBlendAttachment = colorBlendAttachment;
}

void celestia::Pipeline::createVertexInputStateCreateInfo(
	utils::CustomVertexInputAttributeDescriptionFactory& attributeDescriptions,
	const VkVertexInputBindingDescription& bindingDescription,
	uint32_t count)
{
	VkPipelineVertexInputStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.vertexBindingDescriptionCount = count;
	info.pVertexBindingDescriptions = &bindingDescription;
	info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.getSize());
	info.pVertexAttributeDescriptions = attributeDescriptions.rawData();

	builder.vertexInputInfo = info;
}

VkPipelineLayoutCreateInfo celestia::Pipeline::createLayoutInfo(ShaderObject& shader, VkDescriptorSetLayout* descriptors)
{
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;

	if (descriptors != nullptr)
	{
		info.setLayoutCount = 1; // TODO: descriptoreille funktio joka antaa niitten m‰‰r‰n.
		info.pSetLayouts = descriptors;
	}
	
	info.pushConstantRangeCount = 1;
	info.pPushConstantRanges = &shader.getPushConstant();
	return info;
}


VkPipeline celestia::BuildPipeline::buildPipeline(VkDevice device, VkRenderPass pass)
{
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = nullptr;

	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.pNext = nullptr;

	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = nullptr;

	pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	
	VkPipeline newPipeline;
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
	{
		std::cout << "failed to create pipeline\n";
		return VK_NULL_HANDLE;
	}
	else
	{
		return newPipeline;
	}

}
