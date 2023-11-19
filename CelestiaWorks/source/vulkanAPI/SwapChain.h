#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif


namespace celestia
{
	constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	class Device;
	class Window;

	class CELESTIA_WORKS SwapChain
	{
	public:
		SwapChain(Device& device,Window& window);
		~SwapChain();
		SwapChain(const SwapChain&) = delete;
		SwapChain& operator = (const SwapChain&) = delete;

		void recreateSwapChain();
		//VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
		//VkRenderPass getRenderPass() { return renderPass; }
		//VkImageView getImageView(int index) { return swapChainImageViews[index]; }
		//size_t imageCount() { return swapChainImages.size(); }
		//VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
		//VkExtent2D getSwapChainExtent() { return swapChainExtent; }
		//float getSwapChainExtentWidth() { return (float)swapChainExtent.width; }
		//float getSwapChainExtentHeight() { return (float)swapChainExtent.height; }
		//VkSemaphore getImageAvailableSemaphore(int index) { return imageAvailableSemaphores[index]; }
		//VkSemaphore getRenderFinishedSemaphore(int index) { return renderFinishedSemaphores[index]; }
		//VkFence& getInFlightFence(int index) { return inFlightFences[index]; }
		//VkSwapchainKHR& getSwapchain() { return swapChain; }
	private:
		Device& device;
		VkSwapchainKHR swapChain;
		VkFormat imageFormat;
		VkExtent2D extent;
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

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector < VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		/* depth buffer sharts...
		VkImageCreateInfo imageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
		VkImageViewCreateInfo imageviewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
		*/
	};
}
