#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <queue>
#include <functional>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>


struct VmaAllocator_T;

namespace celestia
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete();
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct DeletionQueue
	{
		std::deque<std::function<void()>> deletors;

		void pushFunction(std::function<void()>&& function);

		void flush();
	};

	struct VulkanContenxt
	{
		VkSurfaceKHR surface;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkCommandPool commandPool;
		VmaAllocator_T* allocator;
		DeletionQueue deletionQueue;
	};


	class Window;

	class Device
	{
	public:
		Device(Window& window);
		~Device();
		Device(const Device&) = delete;
		Device& operator = (const Device&) = delete;
		void print() { std::cout << "lol\n"; };

		SwapChainSupportDetails getSwapChainSupport();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	public:
		/*
		VkSurfaceKHR surface;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkCommandPool commandPool;
		VmaAllocator_T* allocator;
		DeletionQueue deletionQueue;
		*/
		static inline VulkanContenxt context;

	private:
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
	private:
		void createInstance();
		void createDebugMessenger();
		void createSurface(Window& window);
		void createDevice();
		void createCommandPool();
		void createAllocator();

		bool supportLayers();
		void supportedExtensions();
		std::vector<const char*> getExtensions();

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);

		void pickPhysicalDevice();

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void destroyDebugUtilsMessengerEXT(VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);

		VkResult createDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	};
}
