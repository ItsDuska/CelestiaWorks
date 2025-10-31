#include "RenderBackend.hpp"
#include "Backend/Window/WindowContext.hpp"
#include "Backend/VulkanAPI/Core/Device.hpp"
#include "Backend/VulkanAPI/Core/SwapChain.hpp"
#include "Backend/VulkanAPI/Core/Pipeline.hpp"
#include "Backend/VulkanAPI/Core/Image.hpp"
#include "Graphics/RenderTarget.hpp"

#include "Backend/Utils/Utils.hpp"
#include "Math/MatrixMath.hpp"

#include <thread>

celestia::Render::Render()
{
	device = std::make_unique<Device>();
	swapChain = std::make_unique<SwapChain>(*device);
	image = std::make_unique<Image>();
	rendering = false;
	clearColor = {0.f, 0.f, 0.f, 1.f};
	hasBindedTEMP = false;
	createCommandBuffers();
	createOffscreenCommandBuffer();
	imageIndex = 0;
	isOffscreenRendering = false;

	PlatformWindow* window = WindowContext::get();

	constants.projection = math::ortho(0.f, static_cast<float>(window->getWindowSize().x), 0.f,
	  static_cast<float>(window->getWindowSize().y), -1.f, 1.f);

	const int DEFAULT_FRAMERATE = 30;

	setFramerateLimit(DEFAULT_FRAMERATE);

	beginFrameTime = std::chrono::system_clock::now();
	endFrameTime = beginFrameTime + framerateLimit;
	// std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> prev_time_in_seconds =
	// std::chrono::time_point_cast<std::chrono::seconds>(beginFrameTime);
}

celestia::Render::~Render()
{
}

void celestia::Render::submitIndexedDraw(DrawInfo& info)
{
	if(!rendering)
	{
		std::cerr << "BeginRenderPass was not called!\n";
		return;
	}

	VkCommandBuffer currentCommandBuffer = isOffscreenRendering ? offscreenCommandBuffer : commandBuffers[currentFrame];

	vkCmdBindPipeline(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, info.material.pipeline);

	vkCmdBindDescriptorSets(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, info.material.layout, 0, 1,
	  &info.descriptor, 0, nullptr);

	// TODO: tee t�� kivemmi.
	// Mat4 model(1.f);
	// UniformBufferObject uniform{};
	// uniform.projection = projection;
	// uniform.transform = model;

	// memcpy(buffer->uniformBuffersMapped[currentFrame], &uniform, sizeof(UniformBufferObject));

	// TODO: add custom push Constants to this
	vkCmdPushConstants(currentCommandBuffer, info.material.layout, VK_SHADER_STAGE_VERTEX_BIT, 0,
	  sizeof(PUSH_CONSTANTS), &constants);

	VkDeviceSize offset = 0;

	vkCmdBindVertexBuffers(currentCommandBuffer, 0, 1, &info.mesh->vertexBuffer.buffer, &offset);
	vkCmdBindIndexBuffer(currentCommandBuffer, info.mesh->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(currentCommandBuffer, info.amountToDraw, 1, 0, 0, 0);
}

void celestia::Render::beginRendering()
{
	if(rendering)
	{
		std::cerr << "BeginRenderPass was already called!\n";
		return;
	}

	isOffscreenRendering = (activeRenderTarget != nullptr);

	if(!isOffscreenRendering)
	{
		// Window rendering: handle swapchain synchronization
		vkWaitForFences(Device::context.device, 1, &swapChain->getInFlightFence(currentFrame), VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(Device::context.device, swapChain->getSwapchain(), UINT64_MAX,
		  swapChain->getImageAvailableSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);

		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			resize();
			return;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to present swap chain image!\n");
		}

		vkResetFences(Device::context.device, 1, &swapChain->getInFlightFence(currentFrame));
		vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	}
	else
	{
		// Offscreen rendering: use separate command buffer
		// Command buffer reset is handled after GPU completion in endRendering
	}

	VkCommandBuffer currentCommandBuffer = isOffscreenRendering ? offscreenCommandBuffer : commandBuffers[currentFrame];

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if(vkBeginCommandBuffer(currentCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

	if (activeRenderTarget)
	{
		renderPassInfo.renderPass = activeRenderTarget->getRenderPass();
		renderPassInfo.framebuffer = activeRenderTarget->getFramebuffer();
		renderPassInfo.renderArea.extent = { (uint32_t)activeRenderTarget->getSize().x, (uint32_t)activeRenderTarget->getSize().y};
	}
	else
	{
		renderPassInfo.renderPass = swapChain->getRenderPass();
		renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);
		renderPassInfo.renderArea.extent = swapChain->extent;
	}

	renderPassInfo.renderArea.offset = {0, 0};

	VkClearValue clearValue = {{{clearColor.x, clearColor.y, clearColor.z, clearColor.w}}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(currentCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	this->rendering = true;
}

void celestia::Render::endRendering()
{
	if(!rendering)
	{
		std::cerr << "BeginRenderPass was not called!\n";
		return;
	}

	VkCommandBuffer currentCommandBuffer = isOffscreenRendering ? offscreenCommandBuffer : commandBuffers[currentFrame];

	vkCmdEndRenderPass(currentCommandBuffer);

	if(vkEndCommandBuffer(currentCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer!");
	}

	if(!isOffscreenRendering)
	{
		// Window rendering: full swapchain synchronization
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {swapChain->getImageAvailableSemaphore(currentFrame)};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

		VkSemaphore signalSemaphore[] = {swapChain->getRenderFinishedSemaphore(currentFrame)};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphore;

		if(vkQueueSubmit(Device::context.graphicsQueue, 1, &submitInfo, swapChain->getInFlightFence(currentFrame)) !=
		   VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		// Present to swapchain
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphore;

		VkSwapchainKHR swapChains[] = {swapChain->getSwapchain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		VkResult result = vkQueuePresentKHR(Device::context.presentQueue, &presentInfo);

		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
		   WindowContext::get()->isFramebufferResized())
		{
			resize();
		}
		else if(result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!\n");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}
	else
	{
		// Offscreen rendering: simple submission
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &offscreenCommandBuffer;

		if(vkQueueSubmit(Device::context.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit offscreen command buffer!");
		}

		// Wait for GPU to complete offscreen work before reusing command buffer
		vkQueueWaitIdle(Device::context.graphicsQueue);
		
		// Reset the offscreen command buffer state for next use
		vkResetCommandBuffer(offscreenCommandBuffer, 0);
	}

	rendering = false;

	/*
	//TEMP
	auto timeInSeconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
	++frameCountPerSecond;
	if (timeInSeconds > prev_time_in_seconds)
	{
		std::cerr << frameCountPerSecond << " frames per second\n";
		frameCountPerSecond = timeInSeconds;
	}
	//TEMP
	*/
	std::this_thread::sleep_until(endFrameTime);
	beginFrameTime = endFrameTime;
	endFrameTime = beginFrameTime + framerateLimit;
}

void celestia::Render::setClearColor(celestia::Color& color)
{
	clearColor = utils::normalizeColor(color);
}

void celestia::Render::cleanUp()
{
	vkDeviceWaitIdle(Device::context.device);
	vkFreeCommandBuffers(
	  Device::context.device, Device::context.commandPool, MAX_FRAMES_IN_FLIGHT, commandBuffers.data());
	vkFreeCommandBuffers(
	  Device::context.device, Device::context.commandPool, 1, &offscreenCommandBuffer);
	Device::context.deletionQueue.flush();
}

void celestia::Render::setFramerateLimit(const int frameRate)
{
	framerateLimit =
	  std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{1. / frameRate});
}

void celestia::Render::setRenderTarget(RenderTarget* target)
{
	activeRenderTarget = target;
}

void celestia::Render::resize()
{
	PlatformWindow* window = WindowContext::get();

	window->setFramebufferResized(false);
	swapChain->recreateSwapChain();

	constants.projection = math::ortho(0.f, static_cast<float>(window->getWindowSize().x), 0.f,
	  static_cast<float>(window->getWindowSize().y), -1.f, 1.f);
}

void celestia::Render::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Device::context.commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if(vkAllocateCommandBuffers(Device::context.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void celestia::Render::createOffscreenCommandBuffer()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Device::context.commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if(vkAllocateCommandBuffers(Device::context.device, &allocInfo, &offscreenCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate offscreen command buffer!");
	}
}
