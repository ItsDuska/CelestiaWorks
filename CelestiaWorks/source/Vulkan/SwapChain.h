#pragma once
#include "Vulkan/Device.h"

namespace celestia
{
	class SwapChain
	{
	public:
		SwapChain(std::shared_ptr<Device> device);
		~SwapChain();
		void recreateSwapChain();

		void createFramebuffers(VkRenderPass renderPass); // t‰‰ on v‰h‰ funky ass
	private:
		void createSwapChain();
		void cleanupSwapChain();
		void createImageViews();


		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector < VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkImage> swapChainImages;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::shared_ptr<Device> device;
	};

}


// TODO: 
//		CREATE BUFFER CLASS