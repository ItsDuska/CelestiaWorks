#pragma once
#include "SwapChain.h"
#include "Pipeline.h"
#include "vulkan/Descriptor.h"
#include <glm/gtx/transform.hpp>
#include <chrono>
/*
	Handlaa kaiken renderaamiseen aka simppelit funtiot
	For future Olli:
		lol luuseri joudut miettim‰‰n miten simppeli objekti saadaan piirretty‰ simppelill‰ tavalla


low level renderer:
	https://youtu.be/uGRSTRGlZVs?t=419 -> 13.00

	public:
	constructor (&window, &device)

	VkRenderPass getSwapChainRenderPass() {return swapChain->getRebderPass();}
	bool isFrameInProgress() {return isFrameStarted;}

	VkCommandBuffer getCurrentCommandBuffer() {return commandBuffers[currentImageIndex];}

	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();

	Window& window;
	Device& device;
	unique_ptr<SwapChain> swapChain;
	vector<VkCommandBuffer> commandBuffers;

	uint32_t currentImageIndex;
	bool isFrameStarted;

*/





/*

UUS IDEA:
RENDER OTTAA KAIKKI TARVITTAVAT VULKAN HOMMAT:
draw call toteutuu t‰‰ll‰

T‰nne pipeline

*/

namespace celestia
{

	class Render
	{
	public:
		Render(Device &device, SwapChain& swapChain,Descriptor& descriptor);
		~Render();
		void drawObjects(std::vector<RenderObject>& first, int count);
	private:
		void createCommandBuffers();
		void recordCommandBuffers(VkCommandBuffer commandBuffer,
			uint32_t imageIndex,
			std::vector<RenderObject>& first,
			int count);

		std::vector<VkCommandBuffer> commandBuffers;
		Device& device;
		SwapChain& swapChain;
		Descriptor& descriptor;
		uint32_t currentFrame;

	};

}