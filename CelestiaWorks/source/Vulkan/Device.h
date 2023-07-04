#pragma once
#include "window/Window.h"

namespace celestia {

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};	

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


	class Device
	{
	public:
		Device(Window &window);
		~Device();
		Device(const Device&) = delete;
		Device& operator = (const Device&) = delete;


		void getFramebufferSize(int* width, int* height);
		VkPhysicalDevice getPhysicalDevice();
		VkDevice getDevice();
		VkSurfaceKHR getSurface();
		VkCommandPool getCommandPool() { return commandPool; }
		VkQueue getGraphicsQueue() { return graphicsQueue; }
		VkQueue getPresentQueue() { return presentQueue; }
		SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }

		//uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
		//VkFormat findSupportedFormat(
			//const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	private:
		void createInstance();
		void createDebugMessenger();
		void createDevice();
		void createSurface();
		void createCommandPool();

		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool supportLayers();
		void supportedExtensions();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		std::vector<const char*> getExtensions();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
		 
		VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);

		

		// variables

		Window& window;
		VkInstance instance;
		VkDevice device;
		VkSurfaceKHR surface;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkCommandPool commandPool;

		// ehkä device samaan hommaan ja niistä struct?
	};


}

/* TODO:
	public:
	VkCommandPool getCommandPool() { return commandPool; }
	VkDevice device() { return device_; }
	VkSurfaceKHR surface() { return surface_; }
	VkQueue graphicsQueue() { return graphicsQueue_; }
	VkQueue presentQueue() { return presentQueue_; }
	SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
	VkFormat findSupportedFormat(
	  const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);


	private:
	 void createCommandPool();

*/

