#pragma once
// Tänne varmaan tarvii CELESTIA_WORKS homman ShaderType Enumille? guh.


namespace celestia
{
	enum class ShaderType : char
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		COMPUTE_SHADER,
		GEOMETRY_SHADER
	};

	enum class RenderGroup
	{
		SPRITE_BATCH,
		TEXT_BATCH,
		BASIC,
		CUSTOM
	};

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
		VEC4_I8,
		VEC4_U8,

		VEC3_I8,
		VEC3_U8,

		VEC2_I8,
		VEC2_U8,

		I8,
		U8,
	};

}