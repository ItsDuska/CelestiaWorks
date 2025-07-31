#pragma once
#include "Texture.h"
#include "ShaderProgram.h"

namespace celestia
{
	// Render options. Defines how things are in the pipeline, what shaders to use
	struct RenderPipeline
	{
		const Texture* texture;
		const ShaderProgram* shader;
	};
}
