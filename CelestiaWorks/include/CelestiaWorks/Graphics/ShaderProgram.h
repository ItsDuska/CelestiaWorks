#pragma once
#include <string>
#include "ShaderTypes.h"
#include <memory>
#include <filesystem> //add support for this

namespace celestia
{
	class ShaderProgramImpl;
	enum class DescriptorType;


	
	class ShaderProgram
	{
	public:
		// Loads the shader and designate it as the shadertype
		void loadShader(const std::string& filepath, ShaderType format);
		void loadShader(const std::filesystem::path& filepath, ShaderType format);
		void loadShader(std::string_view& filepath, ShaderType format);

		// Pushconstants, by default this is used for the projection matrix
		template<typename PushConstantStruct>
		void addPushConstant(int offset, ShaderType dataDestination);

		// register an image, a buffer or something else to the shader.
		// for the offset, use the offset(struct, member) macro.
		void addBinding(uint32_t binding, ShaderUniformTypes format, uint32_t offset);


	private:
		std::unique_ptr<ShaderProgramImpl> impl;
	};


	/*
	class Texture;
	//TEMP IDEOINTI: REMOVE LATER
	class RenderState
	{
		
		ShaderProgram shader;
		Texture *texture;
	};
	*/
}

