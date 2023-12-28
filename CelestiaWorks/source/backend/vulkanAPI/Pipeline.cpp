#include "Pipeline.h"
#include "Device.h"
#include "SwapChain.h"
#include "ShaderObject.h"
#include "Descriptor.h"
#include "backend/utils/Utils.h"
#include "celestiaTypes/CelestiaTypes.h"


celestia::Pipeline::Pipeline(SwapChain& swapChain, Descriptor& descriptor)
	: swapChain(swapChain),descriptor(descriptor)
{
	const char* defaultVertexShader = "../CelestiaWorks/shaders/vert.spv";
	const char* defaultFragmentShader = "../CelestiaWorks/shaders/frag.spv";
	
	ShaderObject shader;
	shader.loadShader(defaultVertexShader,ShaderType::VERTEX_SHADER);
	shader.loadShader(defaultFragmentShader, ShaderType::FRAGMENT_SHADER);
	shader.createPushConstants<PUSH_CONSTANTS>(0, ShaderType::VERTEX_SHADER);

	createPipeline(defaultMaterial, shader, DrawingMode::TRIANGLE, &descriptor);
}

celestia::Pipeline::~Pipeline()
{
	vkDestroyPipeline(Device::context.device, defaultMaterial.pipeline, nullptr);
	vkDestroyPipelineLayout(Device::context.device, defaultMaterial.layout, nullptr);
}

//Use nullptr for descriptor if not using any uniform buffers or textures.
void celestia::Pipeline::createPipeline(Material& material,ShaderObject &shader, DrawingMode drawMode, Descriptor* descriptors)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = createLayoutInfo(shader, descriptors);

	if (vkCreatePipelineLayout(Device::context.device, &pipelineLayoutInfo, nullptr, &material.layout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}

	auto bindingDescription = utils::getBindingDescription();
	auto attributeDescriptions = utils::getAttributeDescription();

	VkPipelineVertexInputStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.vertexBindingDescriptionCount = 1;
	info.pVertexBindingDescriptions = &bindingDescription;
	info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	info.pVertexAttributeDescriptions = attributeDescriptions.data();

	builder.shaderStages = shader.getInfos();
	builder.vertexInputInfo = info;
	builder.inputAssembly = createInputAssembly(drawMode);
	builder.viewport = createViewport();
	builder.scissor = createScissors();

	//builder.depthStencil = depthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS); // VK_COMPARE_OP_LESS_OR_EQUAL
	//pipelineBuilder.dynamicInfo = createDynamicState();

	builder.rasterizer = createRasterizer(drawMode);
	builder.multisampling = createMultisampling();
	builder.colorBlendAttachment = createColorBlendAttachment();
	builder.pipelineLayout = material.layout;

	material.pipeline = builder.buildPipeline(Device::context.device, swapChain.getRenderPass());
}

celestia::Material *celestia::Pipeline::getDefaultMaterial()
{
	return &defaultMaterial;
}

VkPipelineInputAssemblyStateCreateInfo celestia::Pipeline::createInputAssembly(DrawingMode mode)
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

	return info;
}

VkViewport celestia::Pipeline::createViewport()
{
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChain.extent.width);
	viewport.height = static_cast<float>(swapChain.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	return viewport;
}

VkRect2D celestia::Pipeline::createScissors()
{
	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = swapChain.extent;
	return scissor;
}

VkPipelineRasterizationStateCreateInfo celestia::Pipeline::createRasterizer(DrawingMode mode)
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

	return info;
}

VkPipelineMultisampleStateCreateInfo celestia::Pipeline::createMultisampling()
{
	VkPipelineMultisampleStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.sampleShadingEnable = VK_FALSE;
	info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	info.minSampleShading = 1.0f;
	info.pSampleMask = nullptr;
	info.alphaToCoverageEnable = VK_FALSE;
	info.alphaToOneEnable = VK_FALSE;

	return info;
}

VkPipelineColorBlendAttachmentState celestia::Pipeline::createColorBlendAttachment()
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	return colorBlendAttachment;
}

VkPipelineLayoutCreateInfo celestia::Pipeline::createLayoutInfo(ShaderObject& shader, Descriptor* descriptor)
{
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;

	if (descriptor != nullptr)
	{
		info.setLayoutCount = 1; // TODO: descriptoreille funktio joka antaa niitten määrän.
		info.pSetLayouts = &descriptor->getDescriptorSetLayout();
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
