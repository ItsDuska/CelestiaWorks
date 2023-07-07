#include "Render.h"

celestia::Render::Render(Device& device, SwapChain& swapChain, Descriptor& descriptor)
	: device{device}, swapChain{swapChain},descriptor{descriptor}
{
	createCommandBuffers();
}

celestia::Render::~Render()
{
	vkFreeCommandBuffers(device.getDevice(), device.getCommandPool(), MAX_FRAMES_IN_FLIGHT, commandBuffers.data());
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

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	
}

void celestia::Render::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

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

	VkClearValue depthClear;
	depthClear.depthStencil.depth = 1.f;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 2;

	VkClearValue clearValues[] = { clearColor, depthClear };
	renderPassInfo.pClearValues = &clearValues[0];

	Mesh* lastMesh = nullptr;
	Material* lastMaterial = nullptr;

	//HASSU HASSU UNIFORM BUFFER CALC...
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	float rotata =  std::sin((double)0.3*time);

	glm::vec3 camPos = { rotata,0.f,-3.f-rotata };

	glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	//camera projection
	glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
	//glm::mat4 projection = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	projection[1][1] *= -1;

	//fill a GPU camera data struct
	CameraData camData;
	camData.proj = projection;
	camData.view = view;
	camData.viewproj = projection * view;

	FrameData& frameData = descriptor.getFrameData(currentFrame);
	VkDeviceSize bufferSize = sizeof(CameraData);


	void* data;
	vkMapMemory(device.getDevice(), frameData.cameraBuffer.bufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, &camData, (size_t)bufferSize);
	vkUnmapMemory(device.getDevice(), frameData.cameraBuffer.bufferMemory);

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	for (int i = 0; i < count; i++)
	{

		//only bind the pipeline if it doesn't match with the already bound one
		if (first[i].material != lastMaterial) {

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, first[i].material->pipeline);
			lastMaterial = first[i].material;
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				first[i].material->pipelineLayout, 0, 1, &frameData.globalDescriptor, 0, nullptr);
		}

		glm::vec3 camPos1 = { 0.f,0.f,-2.f };

		glm::mat4 view1 = glm::translate(glm::mat4(1.f), camPos1);
		//camera projection
		glm::mat4 projection1 = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
		projection1[1][1] *= -1;
		//model rotation
		glm::mat4 model1 = glm::rotate(glm::mat4{ 1.0f }, time * glm::radians(45.f), glm::vec3(1, 1, 0));

		//calculate final mesh matrix
		glm::mat4 mesh_matrix = projection1 * view1 * model1;




		MeshPushConstants constants;
		//constants.renderMatrix = first[i].transformMatrix;
		constants.renderMatrix =mesh_matrix;


		//upload the matrix to the GPU via push constants
		vkCmdPushConstants(commandBuffer, first[i].material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);
		

		//only bind the mesh if it's a different one from last bind
		if (first[i].mesh != lastMesh) {
			//bind the mesh vertex buffer with offset 0
			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &first[i].mesh->vertexAllocation.buffer, &offset);
			vkCmdBindIndexBuffer(commandBuffer, first[i].mesh->indiciesAllocation.buffer, 0, VK_INDEX_TYPE_UINT16);
			lastMesh = first[i].mesh;
		}

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(first[i].mesh->indices.size()), 1, 0, 0, 0);


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

		
	}

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer!");
	}
}
