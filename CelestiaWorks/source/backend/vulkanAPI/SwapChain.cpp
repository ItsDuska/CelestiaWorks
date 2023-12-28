#include "SwapChain.h"
#include "backend/vulkanAPI/Device.h"
#include "backend/window/window.h"
#include "backend/vulkanAPI/config/VulkanConfig.h"

#include <algorithm>
#include <limits>
#include <array>

celestia::SwapChain::SwapChain(Device& device, Window& window)
	: device(device), window(window)
{
	createSwapChain();
	createImageViews();
	createRenderPass();
	createFramebuffers();
	createSyncObjects();
}

celestia::SwapChain::~SwapChain()
{
	cleanupSwapChain();
	vkDestroyRenderPass(Device::context.device, renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(Device::context.device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(Device::context.device, renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(Device::context.device, inFlightFences[i], nullptr);
	}
}

void celestia::SwapChain::recreateSwapChain()
{
	Vec2i winSize = window.getWindowSize();
	while (winSize.x == 0 || winSize.y == 0)
	{
		winSize = window.getWindowSize();
		window.processMessages();
	}

	vkDeviceWaitIdle(Device::context.device);

	cleanupSwapChain();
	createSwapChain();
	createImageViews();
	createFramebuffers();
}

VkRenderPass celestia::SwapChain::getRenderPass()
{
	return renderPass;
}

VkSemaphore celestia::SwapChain::getImageAvailableSemaphore(int index)
{
	return imageAvailableSemaphores[index];
}

VkSemaphore celestia::SwapChain::getRenderFinishedSemaphore(int index)
{
	return renderFinishedSemaphores[index];
}

VkFence& celestia::SwapChain::getInFlightFence(int index)
{
	return inFlightFences[index];
}

VkSwapchainKHR& celestia::SwapChain::getSwapchain()
{
	return swapChain;
}

VkFramebuffer celestia::SwapChain::getFrameBuffer(int index)
{
	return framebuffers[index];
}

VkImageView celestia::SwapChain::createImageView(VkImage image, VkFormat format)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(Device::context.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create texture image view!\n");
	}

	return imageView;
}

void celestia::SwapChain::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = device.getSwapChainSupport();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	extent = chooseSwapExtent(swapChainSupport.capabilities);
	imageFormat = surfaceFormat.format;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = Device::context.surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = device.findQueueFamilies(Device::context.physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(Device::context.device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(Device::context.device, swapChain, &imageCount, nullptr);
	images.resize(imageCount);
	vkGetSwapchainImagesKHR(Device::context.device, swapChain, &imageCount, images.data());
}

void celestia::SwapChain::createImageViews()
{
	imageViews.resize(images.size());

	for (size_t i = 0; i < images.size(); i++)
	{
		imageViews[i] = createImageView(images[i], imageFormat);
	}
}

void celestia::SwapChain::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = imageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachments[1] = { colorAttachment};
	VkSubpassDependency dependencies[1] = { dependency};

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &attachments[0];
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependencies[0];

	if (vkCreateRenderPass(Device::context.device, &renderPassInfo, nullptr, &renderPass))
	{
		throw std::runtime_error("Failed to create render pass!");
	}
}

void celestia::SwapChain::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(Device::context.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(Device::context.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(Device::context.device,     &fenceInfo,     nullptr, &inFlightFences[i])           != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphores!");
		}
	}
}

void celestia::SwapChain::createFramebuffers()
{
	framebuffers.resize(imageViews.size());

	for (size_t i = 0; i < imageViews.size(); i++)
	{
		std::array<VkImageView, 1> attachments = {
			imageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(Device::context.device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}
}

void celestia::SwapChain::cleanupSwapChain()
{
	for (size_t i = 0; i < framebuffers.size(); i++)
	{
		vkDestroyFramebuffer(Device::context.device, framebuffers[i], nullptr);
	}

	for (size_t i = 0; i < imageViews.size(); i++)
	{
		vkDestroyImageView(Device::context.device, imageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(Device::context.device, swapChain, nullptr);
}

VkSurfaceFormatKHR celestia::SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR celestia::SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D celestia::SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)())
	{
		return capabilities.currentExtent;
	}
	else
	{
		Vec2i winSize = window.getWindowSize();
		VkExtent2D actualExtent = {
			static_cast<uint32_t>(winSize.x),
			static_cast<uint32_t>(winSize.y)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}