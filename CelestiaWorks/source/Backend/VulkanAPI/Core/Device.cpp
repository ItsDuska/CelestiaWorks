#include "Vulkan/Device.hpp"

#ifdef _WIN32
#include "Windows.h"
#endif

#include "Backend/Window/WindowContext.hpp"
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include "Backend/VulkanAPI/Config/VulkanConfig.hpp"

// #define ENABLE_VALIDATION_LAYER // FORCE VALIDATION LAYER FOR RELEASE MODE DEBUGGING

const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation",

};

const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_MAINTENANCE3_EXTENSION_NAME, VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME};

celestia::vk::Device::Device()
{
	createInstance();
#ifdef ENABLE_VALIDATION_LAYER
	createDebugMessenger();
#endif // ENABLE_VALIDATION_LAYER
	createSurface();
	createDevice();
	createCommandPool();
	createAllocator();
}

celestia::vk::Device::~Device()
{
	// deletionQueue.flush();
	vmaDestroyAllocator(context.allocator);
	vkDestroyCommandPool(context.device, context.commandPool, nullptr);
	vkDestroyDevice(context.device, nullptr);

#ifdef ENABLE_VALIDATION_LAYER
	destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif // ENABLE_VALIDATION_LAYER

	vkDestroySurfaceKHR(instance, context.surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

celestia::vk::SwapChainSupportDetails celestia::vk::Device::getSwapChainSupport()
{
	return querySwapChainSupport(context.physicalDevice);
}

void celestia::vk::Device::destroyDebugUtilsMessengerEXT(
  VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if(func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

void celestia::vk::Device::createInstance()
{
#ifdef ENABLE_VALIDATION_LAYER
	if(!supportLayers())
	{
		throw std::runtime_error("Validation layers requested, but not available.\n");
	}
#endif // ENABLE_VALIDATION_LAYER

	supportedExtensions();

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "CelestiaWorks";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "CelestiaWorks Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

#ifdef ENABLE_VALIDATION_LAYER
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();

	populateDebugMessengerCreateInfo(debugCreateInfo);

	createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;
#endif

	if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan instance!");
	}
}

void celestia::vk::Device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
							 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;
}

void celestia::vk::Device::createDebugMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if(createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to set up debug messenger!");
	}
}

void celestia::vk::Device::createSurface()
{
	PlatformWindow* window = WindowContext::get();

#ifdef _WIN32
	// Windows
	VkWin32SurfaceCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.pNext = nullptr;
	info.flags = 0;
	info.hinstance = static_cast<HINSTANCE>(window->getNativeInstance());
	info.hwnd = static_cast<HWND>(window->getNativeHandle());

	if(vkCreateWin32SurfaceKHR(instance, &info, nullptr, &context.surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create VkSurface for Win32!\n");
	}
#elif __linux__
	// Wayland
	VkWaylandSurfaceCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
	info.display = static_cast<wl_display*>(window->getNativeInstance());
	info.surface = static_cast<wl_surface*>(window->getNativeHandle());

	if(vkCreateWaylandSurfaceKHR(instance, &info, nullptr, &context.surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create VkSurface for Wayland!\n");
	}
#endif
}

void celestia::vk::Device::createDevice()
{
	pickPhysicalDevice();

	QueueFamilyIndices indices = findQueueFamilies(context.physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
	float queuePriority = 1.0f;

	for(uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
	deviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
	deviceFeatures.robustBufferAccess = VK_TRUE;

	VkDeviceCreateInfo createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef ENABLE_VALIDATION_LAYER
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#else
	createInfo.enabledLayerCount = 0;

#endif
	VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{};
	descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	descriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
	descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;
	descriptorIndexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
	descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;

	createInfo.pNext = &descriptorIndexingFeatures;

	if(vkCreateDevice(context.physicalDevice, &createInfo, nullptr, &context.device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(context.device, indices.graphicsFamily.value(), 0, &context.graphicsQueue);
	vkGetDeviceQueue(context.device, indices.presentFamily.value(), 0, &context.presentQueue);
}

void celestia::vk::Device::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(context.physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if(vkCreateCommandPool(context.device, &poolInfo, nullptr, &context.commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command pool!");
	}
}

void celestia::vk::Device::createAllocator()
{
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = context.physicalDevice;
	allocatorInfo.device = context.device;
	allocatorInfo.instance = instance;
	vmaCreateAllocator(&allocatorInfo, &context.allocator);
}

VkResult celestia::vk::Device::createDebugUtilsMessengerEXT(VkInstance instance,
  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
  VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if(func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL celestia::vk::Device::debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 6); // keltainen
	std::cout << "VALIDATION LAYER: ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
	// Linux/Wayland: ANSI escape code for yellow
	std::cout << "\033[1;33mVALIDATION LAYER: \033[0m";
#endif

	std::cerr << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

bool celestia::vk::Device::supportLayers()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for(const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for(const auto& layerPropeties : availableLayers)
		{
			if(strcmp(layerName, layerPropeties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if(!layerFound)
		{
			std::cout << "SHART?\n";
			return false;
		}
	}

	return true;
}

void celestia::vk::Device::supportedExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

#ifdef ENABLE_VALIDATION_LAYER
	std::cout << "Available extensions:\n";

	for(const auto& extension : extensions)
	{
		std::cout << "\t-" << extension.extensionName << "\n";
	}

	std::cout << "\n";
#endif // ENABLE_VALIDATION_LAYER
}

std::vector<const char*> celestia::vk::Device::getExtensions() // VK_EXT_DESCRIPTOR_INDEXING_EXTENSION
{
	std::vector<const char*> extensions = {
	  VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};

	// Add platform-specific surface extensions
#ifdef _WIN32
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif __linux__
	extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif

#ifdef ENABLE_VALIDATION_LAYER
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	return extensions;
}

bool celestia::vk::Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for(const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool celestia::vk::Device::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	std::cout << "\t-" << deviceProperties.deviceName << "\n";
	QueueFamilyIndices indices = findQueueFamilies(device);

	// VkPhysicalDeviceFeatures supportedFeatures;
	// vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if(extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void celestia::vk::Device::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if(deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	std::cout << "\nPhysical devices:\n";

	for(const auto& device : devices)
	{
		if(isDeviceSuitable(device))
		{
			context.physicalDevice = device;
			break;
		}
	}

	std::cout << "\n";

	if(context.physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

celestia::vk::QueueFamilyIndices celestia::vk::Device::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;

	for(const auto& queueFamily : queueFamilies)
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, context.surface, &presentSupport);

		if(presentSupport)
		{
			indices.presentFamily = i;
		}

		if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		if(indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

celestia::vk::SwapChainSupportDetails celestia::vk::Device::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, context.surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, context.surface, &formatCount, nullptr);

	if(formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, context.surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, context.surface, &presentModeCount, nullptr);

	if(presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
		  device, context.surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool celestia::vk::QueueFamilyIndices::isComplete()
{
	return graphicsFamily.has_value() && presentFamily.has_value();
}

void celestia::vk::DeletionQueue::pushFunction(std::function<void()>&& function)
{
	deletors.push_back(function);
}

void celestia::vk::DeletionQueue::flush()
{
	for(auto it = deletors.rbegin(); it != deletors.rend(); it++)
	{
		(*it)();
	}

	deletors.clear();
}
