#pragma once
#include <vulkan/vulkan.h>
#include <vector>


namespace celestia
{
	enum class ShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		COMPUTE_SHADER,
		GEOMETRY_SHADER
	};

	class Device;

	class ShaderObject
	{
	public:
		ShaderObject();
		~ShaderObject();
		ShaderObject(const ShaderObject&) = delete;
		ShaderObject& operator = (const ShaderObject&) = delete;

		void loadShader(const char* filepath, ShaderType shader, bool isDefaultShader = false);

		template<typename PushConstantStruct> void createPushConstants(int offset, ShaderType dataDestination)
		{
			pushConstants.offset = offset;
			pushConstants.size = sizeof(PushConstantStruct);
			pushConstants.stageFlags = convertToVkFlags(dataDestination);
		}

		const std::vector<VkPipelineShaderStageCreateInfo> getInfos() const;
		const VkPushConstantRange& getPushConstant() const;
	private:
		VkPushConstantRange pushConstants;
		std::vector<VkShaderModule> shaderModules;
		std::vector< VkPipelineShaderStageCreateInfo> infos;
	private:
		void createShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
		VkShaderStageFlagBits convertToVkFlags(ShaderType type);
	};
}