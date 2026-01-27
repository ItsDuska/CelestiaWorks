#pragma once
#include "../System/Vector.hpp"


struct VkRenderPass_T;
struct VkFramebuffer_T;
struct VkCommandBuffer_T;


namespace celestia
{
	// class Sprite;
	// class VertexBuffer;
	struct RenderPipeline;
	class BatchSpriteRender;
	class TextRender;
	class CustomRender;
	class Drawable;



	using VkRenderPass = VkRenderPass_T*;
	using VkFramebuffer = VkFramebuffer_T*;
	using VkCommandBuffer = VkCommandBuffer_T*;


	/**
	 * @brief Base class for rendering things to the screen or a texture.
	 *
	 */
	class RenderTarget
	{
	public:
		CELESTIA_WORKS virtual ~RenderTarget() = default;

		/**
		 * @brief Basic draw command
		 *
		 * @param drawable The drawable object that has a draw function in it.
		 */
		CELESTIA_WORKS void draw(const Drawable& drawable) const;
		/**
		 * @brief Another draw command with more options via the RenderPipeline struct.
		 *
		 * @param drawable The drawable object that has a draw function in it.
		 * @param pipeline Details about the render pass like texture and shaders.
		 */
		CELESTIA_WORKS void draw(const Drawable& drawable, RenderPipeline& pipeline) const;

		/**
		 * @brief Begins the render.
		 * \note Call this before any draw commands.
		 */
		CELESTIA_WORKS void beginRenderPass();

		/**
		 * @brief Ends the render.
		 * \note Call this after the draw commands.
		 */
		CELESTIA_WORKS void endRenderPass();

		/**
		 * @brief Set the Clear Color.
		 *
		 * @param color The color value via rgba flout values.
		 */
		CELESTIA_WORKS void setClearColor(Color& color);

		CELESTIA_WORKS virtual Vec2i getSize() const = 0;
		//CELESTIA_WORKS bool isValid() const;

	protected:
     	friend class RendererHandler;
      	friend class Render;

      	// small dirty hack. Ignore these ones. They are needed for backend and this was the only way i thought to make these. Bit goofy but it should work.

     	virtual void activateAsRenderTarget() = 0;
		virtual VkRenderPass getRenderPass() const = 0;
		virtual VkFramebuffer getFramebuffer() const = 0;
	};

} // namespace celestia
