#include "RenderBackend.h"
#include "backend/window/Window.h"
#include "backend/vulkanAPI/Device.h"
#include "backend/vulkanAPI/SwapChain.h"
#include "backend/vulkanAPI/Pipeline.h"
#include "backend/vulkanAPI/Buffer.h"
#include "backend/vulkanAPI/Image.h"
#include "backend/vulkanAPI/Descriptor.h"

#include "backend/utils/Utils.h"
#include "math/MatrixMath.h"

#include <thread>

celestia::Render::Render(Window& window)
	: window(window)
{
	device = std::make_unique<Device>(window);
	swapChain = std::make_unique<SwapChain>(*device, window);
	buffer = std::make_unique<Buffer>();
	image = std::make_unique<Image>();
	descriptor = std::make_unique<Descriptor>(*buffer, *image);
	pipeline = std::make_unique<Pipeline>(*swapChain,*descriptor);
	rendering = false;
	clearColor = { 0.f,0.f,0.f,1.f };
	hasBindedTEMP = false;
	createCommandBuffers();
	imageIndex = 0;

	constants.projection = math::ortho(
		0.f,
		static_cast<float>(window.getWindowSize().x),
		0.f,
		static_cast<float>(window.getWindowSize().y),
		-1.f,
		1.f
	);

	const int DEFAULT_FRAMERATE = 30;

	setFramerateLimit(DEFAULT_FRAMERATE);

	beginFrameTime = std::chrono::system_clock::now();
	endFrameTime = beginFrameTime + framerateLimit;
	std::chrono::time_point<std::chrono::system_clock,std::chrono::seconds> prev_time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(beginFrameTime);
}

celestia::Render::~Render()
{
}

void celestia::Render::draw(const Mesh& mesh,const int amountToDraw)
{
	if (!rendering)
	{
		std::cout << "BeginRenderPass was not called!\n";
		return;
	}
	
	if (!hasBindedTEMP)
	{
		vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getDefaultMaterial()->pipeline);

		vkCmdBindDescriptorSets(commandBuffers[currentFrame],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline->getDefaultMaterial()->layout,
			0, 1,
			&descriptor->getDescriptorSet(currentFrame),
			0, nullptr
		);
	}
	
	//static auto startTime = std::chrono::high_resolution_clock::now();
	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//Vec3 rotate{};
	//rotate.x = 0.f;
	//rotate.y = 0.f;
	//rotate.z = -0.5f;

	//const float DEGREE = 360.f;
	//const float angle = fmodf(time * 20, DEGREE);

	Mat4 model(1.f);
	//model = math::translate(model, _pos);

	//model = math::translate(model, Vec3(0.5f * _size.x, 0.5f * _size.y, 0.f));
	//model = math::rotate(model, math::radians(angle), rotate);
	//model = math::translate(model, Vec3(-0.5f * _size.x, -0.5f * _size.y, 0.f));

	//model = math::scale(model, _size);


	UniformBufferObject uniform{};
	//uniform.projection = projection;
	uniform.transform = model;

	memcpy(buffer->uniformBuffersMapped[currentFrame], &uniform, sizeof(UniformBufferObject));

	vkCmdPushConstants(commandBuffers[currentFrame],
		pipeline->getDefaultMaterial()->layout,
		VK_SHADER_STAGE_VERTEX_BIT, 0,
		sizeof(PUSH_CONSTANTS),
		&constants
	);

	if (!hasBindedTEMP)
	{
		VkDeviceSize offset = 0;

		vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, &mesh.vertexBuffer.buffer, &offset);
		vkCmdBindIndexBuffer(commandBuffers[currentFrame], mesh.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

		if (currentFrame == MAX_FRAMES_IN_FLIGHT)
		{
			hasBindedTEMP = true;
		}
	}

	vkCmdDrawIndexed(commandBuffers[currentFrame], amountToDraw, 1, 0, 0, 0);	
}

void celestia::Render::beginRendering()
{
	if (rendering)
	{
		std::cout << "BeginRenderPass was already called!\n";
	}
	

	vkWaitForFences(Device::context.device, 1, &swapChain->getInFlightFence(currentFrame), VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(Device::context.device, swapChain->getSwapchain(), UINT64_MAX,
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

	vkResetFences(Device::context.device, 1, &swapChain->getInFlightFence(currentFrame));
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

	if (vkQueueSubmit(Device::context.graphicsQueue, 1, &submitInfo, swapChain->getInFlightFence(currentFrame)) != VK_SUCCESS)
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

	VkResult result = vkQueuePresentKHR(Device::context.presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.isFramebufferResized()) {
		resize();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	rendering = false;

	//TEMP
	auto time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
	++frame_count_per_second;
	if (time_in_seconds > prev_time_in_seconds)
	{
		std::cerr << frame_count_per_second << " frames per second\n";
		frame_count_per_second = 0;
		prev_time_in_seconds = time_in_seconds;
	}
	//TEMP

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
	vkFreeCommandBuffers(Device::context.device, Device::context.commandPool, MAX_FRAMES_IN_FLIGHT, commandBuffers.data());
	Device::context.deletionQueue.flush();
}

void celestia::Render::setFramerateLimit(const int frameRate)
{
	framerateLimit = std::chrono::round<std::chrono::system_clock::duration>(std::chrono::duration<double>{1. / frameRate});
}

void celestia::Render::resize()
{
	std::cout << "Creating SwapChain Again\n";
	window.setFramebufferResized(false);
	swapChain->recreateSwapChain();

	constants.projection = math::ortho(
		0.f,
		static_cast<float>(window.getWindowSize().x),
		0.f,
		static_cast<float>(window.getWindowSize().y),
		-1.f,
		1.f
	);
}

void celestia::Render::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = Device::context.commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(Device::context.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

