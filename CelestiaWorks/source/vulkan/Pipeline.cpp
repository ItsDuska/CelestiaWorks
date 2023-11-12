#include "Pipeline.h"


celestia::Pipeline::Pipeline(Device& device, SwapChain &swapChain, Descriptor &descriptor)
	: device{device}, swapChain{swapChain}, descriptor{ descriptor }
{
	initPipelines();
}

celestia::Pipeline::~Pipeline()
{
	vkDestroyPipeline(device.getDevice(), meshPipeline, nullptr);
	vkDestroyPipelineLayout(device.getDevice(), meshLayout, nullptr);
}

void celestia::Pipeline::initPipelines()
{
	VkShaderModule vertShaderModule = createShaderModule(readFile("shaders/vert.spv"));
	VkShaderModule fragShaderModule = createShaderModule(readFile("shaders/frag.spv"));

	BuildPipeline pipelineBuilder;

	pipelineBuilder.shaderStages.push_back(
		createShaderStageCreateInfo(
			VK_SHADER_STAGE_VERTEX_BIT,
			vertShaderModule
		)
	);

	pipelineBuilder.shaderStages.push_back(
		createShaderStageCreateInfo(
			VK_SHADER_STAGE_FRAGMENT_BIT,
			fragShaderModule
		)
	);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = createLayoutInfo();

	VkPushConstantRange pushConstant;
	pushConstant.offset = 0;
	pushConstant.size = sizeof(MeshPushConstants);
	pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	pipelineLayoutInfo.pPushConstantRanges = &pushConstant;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptor.getDescriptorSetLayout();

	if (vkCreatePipelineLayout(device.getDevice(), &pipelineLayoutInfo, nullptr, &meshLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescription();

	VkPipelineVertexInputStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.vertexBindingDescriptionCount = 1;
	info.pVertexBindingDescriptions = &bindingDescription;
	info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	info.pVertexAttributeDescriptions = attributeDescriptions.data();


	pipelineBuilder.vertexInputInfo = info;
	pipelineBuilder.inputAssembly = createInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	pipelineBuilder.viewport = createViewport();
	pipelineBuilder.scissor = createScissors();

	pipelineBuilder.depthStencil = depthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS); // VK_COMPARE_OP_LESS_OR_EQUAL

	//pipelineBuilder.dynamicInfo = createDynamicState();
	pipelineBuilder.rasterizer = createRasterizer(VK_POLYGON_MODE_FILL);

	pipelineBuilder.multisampling = createMultisampling();
	pipelineBuilder.colorBlendAttachment = createColorBlendAttachment();
	pipelineBuilder.pipelineLayout = meshLayout;

	meshPipeline = pipelineBuilder.buildPipeline(device.getDevice(), swapChain.getRenderPass());

	createMaterial(meshPipeline, meshLayout, "default");


	vkDestroyShaderModule(device.getDevice(), fragShaderModule, nullptr);
	vkDestroyShaderModule(device.getDevice(), vertShaderModule, nullptr);
}

celestia::Material* celestia::Pipeline::createMaterial(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name)
{
	Material mat;
	mat.pipeline = pipeline;
	mat.pipelineLayout = layout;
	materials[name] = mat;
	return &materials[name];
}

celestia::Material* celestia::Pipeline::getMaterial(const std::string& name)
{
	auto it = materials.find(name);
	if (it == materials.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}

VkShaderModule celestia::Pipeline::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;

	if (vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a shader module!");
	}

	return shaderModule;
}

VkPipelineShaderStageCreateInfo celestia::Pipeline::createShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
	VkPipelineShaderStageCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.module = shaderModule;
	createInfo.stage = stage;
	createInfo.pName = "main";

	return createInfo;
}

VkPipelineInputAssemblyStateCreateInfo celestia::Pipeline::createInputAssembly(VkPrimitiveTopology primitiveTopology)
{
	/*
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : normal triangle drawing
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST : points
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST : line-list
	*/

	VkPipelineInputAssemblyStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.topology = primitiveTopology;
	info.primitiveRestartEnable = VK_FALSE;
	return info;
}

VkViewport celestia::Pipeline::createViewport()
{
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = swapChain.getSwapChainExtentWidth();
	viewport.height = swapChain.getSwapChainExtentHeight();
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	return viewport;
}

VkRect2D celestia::Pipeline::createScissors()
{
	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = swapChain.getSwapChainExtent();
	return scissor;
}

VkPipelineDynamicStateCreateInfo celestia::Pipeline::createDynamicState()
{
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	return dynamicState;
}

VkPipelineDepthStencilStateCreateInfo celestia::Pipeline::depthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp)
{
	VkPipelineDepthStencilStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.pNext = nullptr;

	info.depthTestEnable = bDepthTest ? VK_TRUE : VK_FALSE;
	info.depthWriteEnable = bDepthWrite ? VK_TRUE : VK_FALSE;
	info.depthCompareOp = bDepthTest ? compareOp : VK_COMPARE_OP_ALWAYS;
	info.depthBoundsTestEnable = VK_FALSE;
	info.minDepthBounds = 0.0f;
	info.maxDepthBounds = 1.0f;
	info.stencilTestEnable = VK_FALSE;

	return info;
}

VkPipelineRasterizationStateCreateInfo celestia::Pipeline::createRasterizer(VkPolygonMode polygonMode)
{
	VkPipelineRasterizationStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.depthClampEnable = VK_FALSE; // errori tulee jos t‰n laittaa p‰‰lle emt mit‰ tekee
	//discards all primitives before the rasterization stage if enabled
	info.rasterizerDiscardEnable = VK_FALSE;
	info.polygonMode = polygonMode;
	info.lineWidth = 1.0f;
	//no backface cull
	info.cullMode = VK_CULL_MODE_NONE;  // VK_CULL_MODE_NONE
	info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	//no depth bias
	info.depthBiasEnable = VK_FALSE;
	info.depthBiasConstantFactor = 0.0f;
	info.depthBiasClamp = 0.0f;
	info.depthBiasSlopeFactor = 0.0f;

	return info;
}

VkPipelineMultisampleStateCreateInfo celestia::Pipeline::createMultisampling()
{
	VkPipelineMultisampleStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.sampleShadingEnable = VK_FALSE;
	//multisampling defaulted to no multisampling (1 sample per pixel)
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


VkPipelineLayoutCreateInfo celestia::Pipeline::createLayoutInfo()
{
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;

	//empty defaults
	info.flags = 0;
	info.setLayoutCount = 0;
	info.pSetLayouts = nullptr;
	info.pushConstantRangeCount = 0;
	info.pPushConstantRanges = nullptr;
	return info;
}

std::vector<char> celestia::Pipeline::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("Shart, failed to open a file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
