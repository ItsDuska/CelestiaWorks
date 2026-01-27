#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "CelestiaVulkanTypes.hpp"
#include "System/Vector.hpp"

namespace celestia::vk
{
	class FrameBuffer
	{
	public:
		FrameBuffer(Vec2i size, bool hasDepthBuffer = false, VkFormat colorFormat = VK_FORMAT_R8G8B8A8_UNORM);

		~FrameBuffer();

		void beginRenderPass(VkCommandBuffer cmdBuffer, const Vec4& clearColor);

		void endRenderPass(VkCommandBuffer cmdBuffer);

		const RawTexture* getColorTexture() const
		{
			return &colorTexture;
		};

		const RawTexture* getDepthTexture() const
		{
			return hasDepth ? &depthTexture : VK_NULL_HANDLE;
		}

		VkRenderPass getRenderPass() const
		{
			return renderPass;
		}

		VkFramebuffer getFramebuffer() const
		{
			return frameBuffer;
		}

		Vec2i getSize() const
		{
			return size;
		}

		// Remember to update descriptor sets maybe too.
		void resize(Vec2i newSize);

		bool isValid() const
		{
			return renderPass != VK_NULL_HANDLE;
		}

	private:
		void init();
		void createRenderPass();
		void createFramebuffer();
		void createColorTexture();
		void createDepthTexture();
		void cleanup();

	private:
		Vec2i size;
		VkFormat colorFormat;
		VkFormat depthFormat;
		bool hasDepth;

		VkRenderPass renderPass;
		VkFramebuffer frameBuffer;
		RawTexture colorTexture;
		RawTexture depthTexture;
	};
} // namespace celestia::vk
