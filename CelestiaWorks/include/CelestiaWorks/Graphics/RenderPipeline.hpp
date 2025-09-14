#pragma once
#include "Texture.hpp"
#include "ShaderProgram.hpp"

namespace celestia
{
	// Render options. Defines how things are in the pipeline, what shaders to use
	struct RenderPipeline
	{
		const Texture* texture;
		const ShaderProgram* shader;
	};
} // namespace celestia
