#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <set>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
//#define VMA_IMPLEMENTATION
//#include "vk_mem_alloc.h"

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#ifdef NDEBUG
const bool VALIDATION_LAYERS = false;
#else
const bool VALIDATION_LAYERS = true;
#endif 

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

	class Window;

	class CELESTIA_WORKS Device
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
		VkSurfaceKHR surface;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkCommandPool commandPool;
		VmaAllocator_T* allocator;

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
