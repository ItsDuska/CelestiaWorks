#include "FrameBuffer.hpp"
#include "Backend/VulkanAPI/Core/CelestiaVulkanTypes.hpp"
#include "Device.hpp"
#include "Image.hpp"
#include "System/Vector.hpp"

#include <exception>
#include <stdexcept>
#include <array>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace celestia
{
	FrameBuffer::FrameBuffer(Vec2i size, bool hasDepthBuffer, VkFormat colorFormat)
		: size(size), colorFormat(colorFormat), hasDepth(hasDepthBuffer), renderPass(VK_NULL_HANDLE),
		  frameBuffer(VK_NULL_HANDLE)
	{
		depthFormat = VK_FORMAT_D32_SFLOAT;
		init();
	}

	FrameBuffer::~FrameBuffer()
	{
		cleanup();
	}

	void FrameBuffer::beginRenderPass(VkCommandBuffer cmdBuffer, const Vec4& clearColor)
	{
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{clearColor.x, clearColor.y, clearColor.z, clearColor.w}};

		if(hasDepth)
		{
			clearValues[1].depthStencil = {1.0f, 0}; // stencil ignored :p
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = frameBuffer;
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent.width = size.x;
		renderPassInfo.renderArea.extent.height = size.y;
		renderPassInfo.clearValueCount = hasDepth ? 2 : 1;
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void FrameBuffer::endRenderPass(VkCommandBuffer cmdBuffer)
	{
		vkCmdEndRenderPass(cmdBuffer);
	}

	void FrameBuffer::createRenderPass()
	{
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkAttachmentReference> colorRefs;
		VkAttachmentReference depthRef{};

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = colorFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		attachments.push_back(colorAttachment);
		colorRefs.push_back({0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});

		if(hasDepth)
		{
			VkAttachmentDescription depthAttachment{};
			depthAttachment.format = depthFormat;
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			attachments.push_back(depthAttachment);
			depthRef = {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
		}

		// subpass thingyyyyy

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
		subpass.pColorAttachments = colorRefs.data();
		subpass.pDepthStencilAttachment = hasDepth ? &depthRef : VK_NULL_HANDLE;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		// Use SwapChain-compatible dependencies when using SwapChain format
		if(colorFormat == VK_FORMAT_B8G8R8A8_SRGB)
		{
			// Single dependency compatible with SwapChain
			VkSubpassDependency dependency{};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;
		}
		else
		{
			// Original dual dependency structure for other formats
			std::array<VkSubpassDependency, 2> deps{};
			deps[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			deps[0].dstSubpass = 0;
			deps[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			deps[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			deps[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			deps[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			deps[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			deps[1].srcSubpass = 0;
			deps[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			deps[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			deps[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			deps[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			deps[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			deps[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			renderPassInfo.dependencyCount = static_cast<uint32_t>(deps.size());
			renderPassInfo.pDependencies = deps.data();
		}

		if(vkCreateRenderPass(Device::context.device, &renderPassInfo, nullptr, &renderPass))
		{
			throw std::runtime_error("ERROR: Failed to create render pass for FrameBuffer!\n");
		}
	}

	void FrameBuffer::createColorTexture()
	{
		Image::createImage(size, colorFormat, VK_IMAGE_TILING_OPTIMAL,
		  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		  colorTexture.allocatedImage);

		colorTexture.imageView = Image::createImageView(colorTexture.allocatedImage.image, colorFormat);

		Image::transitionImageLayout(
		  colorTexture.allocatedImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	void FrameBuffer::createDepthTexture()
	{
		Image::createImage(size, depthFormat, VK_IMAGE_TILING_OPTIMAL,
		  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		  depthTexture.allocatedImage);

		depthTexture.imageView =
		  Image::createImageView(depthTexture.allocatedImage.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

		Image::transitionImageLayout(depthTexture.allocatedImage.image, VK_IMAGE_LAYOUT_UNDEFINED,
		  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	void FrameBuffer::createFramebuffer()
	{
		std::vector<VkImageView> attachments;
		attachments.push_back(colorTexture.imageView);

		if(hasDepth)
		{
			attachments.push_back(depthTexture.imageView);
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = size.x;
		framebufferInfo.height = size.y;
		framebufferInfo.layers = 1;

		if(vkCreateFramebuffer(Device::context.device, &framebufferInfo, nullptr, &frameBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create framebuffer!\n");
		}
	}

	void FrameBuffer::cleanup()
	{
		vkDeviceWaitIdle(Device::context.device);
		
		if(frameBuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(Device::context.device, frameBuffer, nullptr);
			frameBuffer = VK_NULL_HANDLE;
		}

		if(colorTexture.allocatedImage.image != VK_NULL_HANDLE)
		{
			Image::deleteTextureImage(colorTexture);
		}
		if(hasDepth && depthTexture.allocatedImage.image != VK_NULL_HANDLE)
		{
			Image::deleteTextureImage(depthTexture);
		}
		if(renderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(Device::context.device, renderPass, nullptr);
		}
	}

	void FrameBuffer::init()
	{
		try
		{
			createRenderPass();
			createColorTexture();
			if(hasDepth)
			{
				createDepthTexture();
			}
			createFramebuffer();
		}
		catch(const std::exception& e)
		{
			cleanup();
			throw;
		}
	}

	void FrameBuffer::resize(Vec2i newSize)
	{
		cleanup();
		size = newSize;
		init();
	}
} // namespace celestia