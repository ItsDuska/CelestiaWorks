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
}