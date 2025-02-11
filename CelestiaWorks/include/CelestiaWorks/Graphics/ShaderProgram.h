#pragma once
#include <string>
#include "Graphics/ShaderTypes.h"
#include <memory>
//#include <filesystem> add support for this

namespace celestia
{
	class ShaderProgramImpl;
	enum class DescriptorType;


	class ShaderProgram
	{
	public:
		// Loads the shader and designate it as the shadertype
		void loadShader(const std::string& filepath, ShaderType format);

		void addBinding(uint32_t bindingSlot, DescriptorType type, ShaderType shader, uint32_t count);


	private:
		std::unique_ptr<ShaderProgramImpl> impl;
	};
}