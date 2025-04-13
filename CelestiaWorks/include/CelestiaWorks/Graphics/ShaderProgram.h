#pragma once
#include <string>
#include "Graphics/ShaderTypes.h"
#include <memory>
#include <filesystem> //add support for this

namespace celestia
{
	class ShaderProgramImpl;
	enum class DescriptorType;


	enum class ShaderUniformTypes
	{
		//64bit
		VEC4_F64,
		VEC4_I64,
		VEC4_U64,

		VEC3_F64,
		VEC3_I64,
		VEC3_U64,

		VEC2_F64,
		VEC2_I64,
		VEC2_U64,

		F64,
		I64,
		U64,

		//32bit
		VEC4_F32,
		VEC4_I32,
		VEC4_U32,

		VEC3_F32,
		VEC3_I32,
		VEC3_U32,

		VEC2_F32,
		VEC2_I32,
		VEC2_U32,

		F32,
		I32,
		U32,

		//16bit
		VEC4_F16,
		VEC4_I16,
		VEC4_U16,

		VEC3_F16,
		VEC3_I16,
		VEC3_U16,

		VEC2_F16,
		VEC2_I16,
		VEC2_U16,

		F16,
		I16,
		U16,

		//8bit
		VEC4_F8,
		VEC4_I8,
		VEC4_U8,

		VEC3_F8,
		VEC3_I8,
		VEC3_U8,

		VEC2_F8,
		VEC2_I8,
		VEC2_U8,

		F8,
		I8,
		U8,	
	};


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

