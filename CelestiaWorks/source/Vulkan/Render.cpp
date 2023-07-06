#include "Render.h"

celestia::Render::Render(Device& device, SwapChain& swapChain)
	: device{device}, swapChain{swapChain}
{
	createCommandBuffers();
}

celestia::Render::~Render()
{
	vkFreeCommandBuffers(device.getDevice(), device.getCommandPool(), swapChain.MAX_FRAMES_IN_FLIGHT, commandBuffers.data());
}

void celestia::Render::drawObjects(std::vector<RenderObject>& first, int count)
{
	vkWaitForFences(device.getDevice(), 1, &swapChain.getInFlightFence(currentFrame) , VK_TRUE, UINT64_MAX);
	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(device.getDevice(), swapChain.getSwapchain(), UINT64_MAX,
		swapChain.getImageAvailableSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		device.setFrameBufferResized(false);
		swapChain.recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to present swap chain image");
	}

	vkResetFences(device.getDevice(), 1, &swapChain.getInFlightFence(currentFrame));

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	recordCommandBuffers(commandBuffers[currentFrame], imageIndex,first,count);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { swapChain.getImageAvailableSemaphore(currentFrame) };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphore[] = { swapChain.getRenderFinishedSemaphore(currentFrame)};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphore;

	if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, swapChain.getInFlightFence(currentFrame)) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphore;

	VkSwapchainKHR swapChains[] = { swapChain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || device.frameBufferResized()) {
		device.setFrameBufferResized(false);
		swapChain.recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % swapChain.MAX_FRAMES_IN_FLIGHT;

	
}

void celestia::Render::createCommandBuffers()
{
	commandBuffers.resize(swapChain.MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void celestia::Render::recordCommandBuffers(VkCommandBuffer commandBuffer, 
	uint32_t imageIndex,
	std::vector<RenderObject>& first,
	int count)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain.getRenderPass();
	renderPassInfo.framebuffer = swapChain.getFrameBuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	Mesh* lastMesh = nullptr;
	Material* lastMaterial = nullptr;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	for (int i = 0; i < count; i++)
	{
		RenderObject& object = first[i];

		//only bind the pipeline if it doesn't match with the already bound one
		if (object.material != lastMaterial) {

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipeline);
			lastMaterial = object.material;
		}


		//glm::mat4 model = object.transformMatrix;
		//final render matrix, that we are calculating on the cpu
		//glm::mat4 mesh_matrix = projection * view * model;

		//MeshPushConstants constants;
		//constants.render_matrix = mesh_matrix;

		//upload the mesh to the GPU via push constants
		//vkCmdPushConstants(cmd, object.material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);

		//only bind the mesh if it's a different one from last bind
		if (object.mesh != lastMesh) {
			//bind the mesh vertex buffer with offset 0
			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &object.mesh->vertexAllocation.buffer, &offset);
			vkCmdBindIndexBuffer(commandBuffer, object.mesh->indiciesAllocation.buffer, 0, VK_INDEX_TYPE_UINT16);
			lastMesh = object.mesh;
		}

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(object.mesh->indices.size()), 1, 0, 0, 0);

		//vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

		/*
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain.getSwapChainExtentWidth());
		viewport.height = static_cast<float>(swapChain.getSwapChainExtentHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);


		VkRect2D scissor{};
		scissor.offset = { 0,0 };
		scissor.extent = swapChain.getSwapChainExtent();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
		VkBuffer vertexBuffers[] = { vertexBuffer };
		*/


		//VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		//vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		//CmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		
	}

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer!");
	}
}
