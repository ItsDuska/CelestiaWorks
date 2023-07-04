#pragma once

/*
	Handlaa kaiken renderaamiseen aka simppelit funtiot
	For future Olli:
		lol luuseri joudut miettim��n miten simppeli objekti saadaan piirretty� simppelill� tavalla


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

class Render
{

};

