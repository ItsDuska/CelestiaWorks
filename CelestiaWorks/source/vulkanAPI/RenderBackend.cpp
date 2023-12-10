#include "RenderBackend.h"
#include "window/Window.h"
#include "vulkanAPI/Device.h"
#include "vulkanAPI/SwapChain.h"
#include "Pipeline.h"
#include "Buffer.h"

#include "CelestiaVulkanTypes.h"
#include "utils/Utils.h"
#include "math/MatrixMath.h"
#include <chrono>

//#include <glm/glm.hpp>
//#include <glm/ext/matrix_transform.hpp>

celestia::Render::Render(Window& window)
	: window(window), imageIndex(0)
{
	device = std::make_unique<Device>(window);
	swapChain = std::make_unique<SwapChain>(*device, window);
	buffer = std::make_unique<Buffer>(*device);
	pipeline = std::make_unique<Pipeline>(*device, *swapChain);
	rendering = false;
	clearColor = { 0.f,0.f,0.f,1.f };

	createCommandBuffers();
}

celestia::Render::~Render()
{
	vkDeviceWaitIdle(device->device);
	vkFreeCommandBuffers(device->device, device->commandPool, MAX_FRAMES_IN_FLIGHT, commandBuffers.data());
	device->deletionQueue.flush();
}

void celestia::Render::draw()
{
	if (!rendering)
	{
		std::cout << "BeginRenderPass was not called!\n";
		return;
	}
	
	vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getDefaultPipeline());

	Vec3 pos{};
	pos.x = 250.f;
	pos.y = 250.f;
	pos.z = 0.f;

	Vec3 size{};
	size.x = 200.f;
	size.y = 200.f;
	size.z = 1.f;

	Vec3 rotate{};
	rotate.x = 0.f;
	rotate.y = 0.f;
	rotate.z = 1.f;

	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	const float DEGREE = 360.f;
	const float angle = fmodf(time*15, DEGREE);


	Mat4 model(1.f);
	model = math::translate(model, pos);

	model = math::translate(model, Vec3(0.5f * size.x, 0.5f * size.y, 0.f));
	model = math::rotate(model, math::radians(angle), rotate);
	model = math::translate(model, Vec3(-0.5f * size.x, -0.5f * size.y, 0.f));

	model = math::scale(model, size);


	Mat4 projection = math::ortho(
		0.f,
		static_cast<float>(window.getWindowSize().x) ,
		static_cast<float>(window.getWindowSize().y) ,
		0.f,
		-1.f,
		1.f
	);

	Mat4 projection1(1.f);

	PUSH_CONSTANTS constants{};
	constants.projection = projection;
	constants.transform = model;


	vkCmdPushConstants(commandBuffers[currentFrame],
		pipeline->getDefaultLayout(),
		VK_SHADER_STAGE_VERTEX_BIT, 0,
		sizeof(PUSH_CONSTANTS),
		&constants
	);

	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, &buffer->getDefaultMesh().vertexBuffer.buffer, &offset);
	vkCmdBindIndexBuffer(commandBuffers[currentFrame], buffer->getDefaultMesh().indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);


	vkCmdDrawIndexed(commandBuffers[currentFrame], buffer->getDefaultMesh().indexBufferSize, 1, 0, 0, 0);
}

void celestia::Render::beginRendering()
{
	if (rendering)
	{
		std::cout << "BeginRenderPass was already called!\n";
	}

	vkWaitForFences(device->device, 1, &swapChain->getInFlightFence(currentFrame), VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(device->device, swapChain->getSwapchain(), UINT64_MAX,
		swapChain->getImageAvailableSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		resize();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to present swap chain image");
	}

	vkResetFences(device->device, 1, &swapChain->getInFlightFence(currentFrame));
	vkResetCommandBuffer(commandBuffers[currentFrame], 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapChain->extent;

	VkClearValue clearValue = { {{clearColor.x, clearColor.y, clearColor.z, clearColor.w}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	this->rendering = true;
}

void celestia::Render::endRendering()
{
	if (!rendering)
	{
		std::cout << "BeginRenderPass was not called!\n";
		return;
	}

	vkCmdEndRenderPass(commandBuffers[currentFrame]);

	if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer!");
	}
	
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { swapChain->getImageAvailableSemaphore(currentFrame) };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphore[] = { swapChain->getRenderFinishedSemaphore(currentFrame) };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphore;

	if (vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, swapChain->getInFlightFence(currentFrame)) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphore;

	VkSwapchainKHR swapChains[] = { swapChain->getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(device->presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.isFramebufferResized()) {
		resize();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	rendering = false;
}

void celestia::Render::setClearColor(celestia::Color& color)
{
	clearColor = utils::normalizeColor(color);
}

void celestia::Render::resize()
{
	std::cout << "Creating SwapChain Again\n";
	window.setFramebufferResized(false);
	swapChain->recreateSwapChain();
}

void celestia::Render::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = device->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}
