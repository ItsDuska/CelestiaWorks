#pragma once
#include <vulkan/vulkan.h>
#include <vector>


namespace celestia
{

	class Device;
	class Window;

	class SwapChain
	{
	public:
		SwapChain(Device& device,Window& window);
		~SwapChain();
		SwapChain(const SwapChain&) = delete;
		SwapChain& operator = (const SwapChain&) = delete;

		void recreateSwapChain();
		VkRenderPass getRenderPass();
		//VkImageView getImageView(int index) { return swapChainImageViews[index]; }
		//size_t imageCount() { return swapChainImages.size(); }
		//VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
		//VkExtent2D getSwapChainExtent() { return swapChainExtent; }
		//float getSwapChainExtentWidth() { return (float)swapChainExtent.width; }
		//float getSwapChainExtentHeight() { return (float)swapChainExtent.height; }
		VkSemaphore getImageAvailableSemaphore(int index);
		VkSemaphore getRenderFinishedSemaphore(int index);
		VkFence& getInFlightFence(int index);
		VkSwapchainKHR& getSwapchain();
		VkFramebuffer getFrameBuffer(int index);

	public:
		VkExtent2D extent;

	private:
		Device& device;
		VkSwapchainKHR swapChain;
		VkFormat imageFormat;
		std::vector<VkImageView> imageViews;
		std::vector<VkImage> images;
		std::vector<VkFramebuffer> framebuffers;
		VkRenderPass renderPass;
		Window& window;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

	private:
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createSyncObjects();
		void createFramebuffers();
		void cleanupSwapChain();

		VkImageView createImageView(VkImage image, VkFormat format);

		

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector < VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		/* depth buffer sharts...
		VkImageCreateInfo imageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
		VkImageViewCreateInfo imageviewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
		*/
	};
}
