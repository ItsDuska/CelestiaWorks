#pragma once
#include "Texture.h"

namespace celestia
{

	// Render options. Defines how things are in the pipeline, what shaders to use
	class RenderPipeline
	{
	public:
		const Texture* texuture;
		const void* Shader;
	};


}
