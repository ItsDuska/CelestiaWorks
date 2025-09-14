#include "ShaderObject.hpp"
#include "Backend/Utils/Utils.hpp"
#include "Device.hpp"

// Sprite rendering
#include "../../../shaders/batchRenderVert.h"
#include "../../../shaders/batchRenderFrag.h"

// Text rendering
#include "../../../shaders/batchTextRenderFrag.h"
#include "../../../shaders/batchTextRenderVert.h"

// Basic render
#include "../../../shaders/basic/basicFrag.h"
#include "../../../shaders/basic/basicVert.h"

celestia::ShaderObject::ShaderObject() : pushConstants({})
{
}

celestia::ShaderObject::~ShaderObject()
{
	for(VkShaderModule& shader : shaderModules)
	{
		vkDestroyShaderModule(Device::context.device, shader, nullptr);
	}

	shaderModules.clear();
}

// Behold, the goofiest way to store default shaders!
static const std::vector<uint32_t> getDefaultShaders(celestia::ShaderType shader, celestia::RenderGroup type)
{
	if(shader == celestia::ShaderType::VERTEX_SHADER)
	{
		if(type == celestia::RenderGroup::TEXT_BATCH)
		{
			const std::vector<uint32_t> vertexBinaryText(
			  batchTextVertexShader, batchTextVertexShader + sizeof(batchTextVertexShader) / sizeof(uint32_t));

			return vertexBinaryText;
		}
		else if(type == celestia::RenderGroup::SPRITE_BATCH)
		{
			const std::vector<uint32_t> vertexBinaryBatch(
			  batchVertexShader, batchVertexShader + sizeof(batchVertexShader) / sizeof(uint32_t));
			return vertexBinaryBatch;
		}
		else if(type == celestia::RenderGroup::BASIC)
		{
			const std::vector<uint32_t> vertexBinaryBasic(
			  basicVertexShader, basicVertexShader + sizeof(basicVertexShader) / sizeof(uint32_t));
			return vertexBinaryBasic;
		}

		std::cerr << "WARNING: Returning empty default vertex shader source code!\n";
		return std::vector<uint32_t>();
	}

	if(type == celestia::RenderGroup::TEXT_BATCH)
	{
		const std::vector<uint32_t> fragmentBinaryText(
		  batchTextFragmentShader, batchTextFragmentShader + sizeof(batchTextFragmentShader) / sizeof(uint32_t));
		return fragmentBinaryText;
	}
	else if(type == celestia::RenderGroup::SPRITE_BATCH)
	{
		const std::vector<uint32_t> fragmentBinaryBatch(
		  batchFragmentShader, batchFragmentShader + sizeof(batchFragmentShader) / sizeof(uint32_t));
		return fragmentBinaryBatch;
	}

	else if(type == celestia::RenderGroup::BASIC)
	{
		const std::vector<uint32_t> fragmentBinaryBasic(
		  basicFragmentShader, basicFragmentShader + sizeof(basicFragmentShader) / sizeof(uint32_t));
		return fragmentBinaryBasic;
	}

	std::cerr << "WARNING: Returning empty default vertex shader source code!\n";
	return std::vector<uint32_t>();
}

void celestia::ShaderObject::loadShader(const char* filepath, ShaderType shader, RenderGroup type, bool isDefaultShader)
{
	std::vector<uint32_t> byteCode;

	if(isDefaultShader)
	{
		byteCode = getDefaultShaders(shader, type);
	}
	else
	{
		byteCode = utils::readFileAsU32(filepath);
	}

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = byteCode.size() * sizeof(uint32_t);
	createInfo.pCode = byteCode.data();

	VkShaderModule shaderModule;

	if(vkCreateShaderModule(Device::context.device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
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
	switch(type)
	{
	case celestia::ShaderType::VERTEX_SHADER: return VK_SHADER_STAGE_VERTEX_BIT;
	case celestia::ShaderType::FRAGMENT_SHADER: return VK_SHADER_STAGE_FRAGMENT_BIT;
	case celestia::ShaderType::COMPUTE_SHADER: return VK_SHADER_STAGE_COMPUTE_BIT;
	case celestia::ShaderType::GEOMETRY_SHADER: return VK_SHADER_STAGE_GEOMETRY_BIT;
	default: break;
	}

	std::cout << "FAILED TO CONVERT VK_FLAGS!\n";
	return VK_SHADER_STAGE_ALL_GRAPHICS;
}
