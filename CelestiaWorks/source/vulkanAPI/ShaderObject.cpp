#include "ShaderObject.h"
#include "utils/Utils.h"
#include "vulkanAPI/Device.h"
#include <iostream>

celestia::ShaderObject::ShaderObject(Device& device)
	:device(device),pushConstants({})
{
}

celestia::ShaderObject::~ShaderObject()
{
	for (VkShaderModule& shader : shaderModules)
	{
		vkDestroyShaderModule(device.device, shader, nullptr);
	}

	shaderModules.clear();
}

void celestia::ShaderObject::loadShader(const char* filepath, ShaderType shader)
{
	std::vector<char> code = utils::readFile(filepath);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;

	if (vkCreateShaderModule(device.device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a shader module!\n");
	}

	shaderModules.push_back(shaderModule);

	createShaderStageCreateInfo(convertToVkFlags(shader), shaderModule);
}

const std::vector<VkPipelineShaderStageCreateInfo> celestia::ShaderObject::getInfos() const
{
	return infos;
}

const VkPushConstantRange& celestia::ShaderObject::getPushConstant() const
{
	return pushConstants;
}

void celestia::ShaderObject::createShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
	VkPipelineShaderStageCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.module = shaderModule;
	createInfo.stage = stage;
	createInfo.pName = "main";
	infos.push_back(createInfo);
}

VkShaderStageFlagBits celestia::ShaderObject::convertToVkFlags(ShaderType type)
{
	switch (type)
	{
	case celestia::ShaderType::VERTEX_SHADER:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case celestia::ShaderType::FRAGMENT_SHADER:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case celestia::ShaderType::COMPUTE_SHADER:
		return VK_SHADER_STAGE_COMPUTE_BIT;
	case celestia::ShaderType::GEOMETRY_SHADER:
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	default:
		break;
	}

	std::cout << "FAILED TO CONVERT VK_FLAGS!\n";
	return VK_SHADER_STAGE_ALL_GRAPHICS;
}

/*
template<typename PushConstantStruct> inline void celestia::ShaderObject::createPushConstants(int offset, ShaderType dataDestination)
{
	pushConstants.offset = offset;
	pushConstants.size = sizeof(PushConstantStruct);
	pushConstants.stageFlags = dataDestination;
}
*/