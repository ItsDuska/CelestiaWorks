#pragma once
#include "RenderPipeline.hpp"

namespace celestia
{
	class RendererHandler;

	/**
	 * @brief Interface to make your own objects drawable on the screen.
	 * \details Add this to your own object ot make it abable to be drawn on the screen.
	 */
	class Drawable
	{
	public:
		/**
		 * @brief Destroy the Drawable object
		 *
		 */
		virtual ~Drawable() = default;

		/**
		 * @brief
		 *
		 * @param renderer
		 * @param pipeline
		 */
		virtual void draw(const RendererHandler& renderer, RenderPipeline* pipeline) const = 0;
	};
} // namespace celestia
