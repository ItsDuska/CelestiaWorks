#pragma once
#include "Texture.hpp"
#include "ShaderProgram.hpp"

namespace celestia
{

	struct PipelineState
	{

	};





	/**
	 * @brief Render options.
	 * Defines how things are in the pipeline, what shaders to use
	 */

	class RenderPipeline
	{
	public:
		RenderPipeline(const Texture* texture, const ShaderProgram* shader)
			: texture(texture), shader(shader)
		{

		}



	//private:
		const Texture* texture;
		const ShaderProgram* shader;
	};
} // namespace celestia
