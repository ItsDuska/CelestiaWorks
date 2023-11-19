#pragma once
#include "Vulkan/Device.h"

namespace celestia
{
	class SwapChain
	{
	public:
		SwapChain(Device& device);
		~SwapChain();
		void recreateSwapChain();

		VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
		VkRenderPass getRenderPass() { return renderPass; }
		VkImageView getImageView(int index) { return swapChainImageViews[index]; }
		size_t imageCount() { return swapChainImages.size(); }
		VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
		VkExtent2D getSwapChainExtent() { return swapChainExtent; }
		float getSwapChainExtentWidth() { return (float)swapChainExtent.width; }
		float getSwapChainExtentHeight() { return (float)swapChainExtent.height; }
		VkSemaphore getImageAvailableSemaphore(int index) { return imageAvailableSemaphores[index]; }
		VkSemaphore getRenderFinishedSemaphore(int index) { return renderFinishedSemaphores[index]; }
		VkFence& getInFlightFence(int index) { return inFlightFences[index]; }
		VkSwapchainKHR& getSwapchain() { return swapChain; }

	private:
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createSyncObjects();
		void createFramebuffers();
		void cleanupSwapChain();
		void allocateDepthBuffer();

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector < VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkImageCreateInfo imageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
		VkImageViewCreateInfo imageviewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkImage> swapChainImages;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		Device &device;
		VkRenderPass renderPass;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		//Depth buffer
		VkImageView depthImageView;
		AllocatedImage depthImage;
		VkFormat depthFormat;
	};

}


/* TODO: MIT� T�M�N CLASSIN PIT�S TEHD�:
	public:
	 SwapChain(Device &deviceRef, VkExtent2D windowExtent);
	 SwapChain(
	  Device &deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);

	constructor();

	VkFrameBuffer getFrameBuffer(int index) { return swapchainFramebuffer[index];}
	getRenderPass()
	get ImageView(int index) { return swapChainImageViews[index];}
	size_t imageCount() {return swapChainImages.size();}
	getSwapChainImageFormat();
	get swapCainExtent();
	getSwapChainExtentWidth();
	getSwapChainExtentHeight();

	VkFormat findDepthFormat();
	VkResult acquireNextImage(uint32_t *imageIndex);
	VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

	bool compareSwapFormats(const LveSwapChain &swapChain) const {
		return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
		   swapChain.swapChainImageFormat == swapChainImageFormat;
	}


	private:
	void init()

	void createSwapChain()
	void createImageViews()
	void createDepthResources()
	void createRenderPass()
	void createFramebuffers()
	void createSyncObjects()


	VkFormat swapChainImageFormat;
	 VkFormat swapChainDepthFormat;
	 VkExtent2D swapChainExtent;

	 std::vector<VkFramebuffer> swapChainFramebuffers;
	 VkRenderPass renderPass;

	 std::vector<VkImage> depthImages;
	 std::vector<VkDeviceMemory> depthImageMemorys;
	 std::vector<VkImageView> depthImageViews;
	 std::vector<VkImage> swapChainImages;
	 std::vector<VkImageView> swapChainImageViews;

	 Device &device;
	 VkExtent2D windowExtent;

	 VkSwapchainKHR swapChain;
	 std::shared_ptr<SwapChain> oldSwapChain;

	 std::vector<VkSemaphore> imageAvailableSemaphores;
	 std::vector<VkSemaphore> renderFinishedSemaphores;
	 std::vector<VkFence> inFlightFences;
	 std::vector<VkFence> imagesInFlight;
	 size_t currentFrame = 0;
*/