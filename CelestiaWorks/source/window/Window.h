#pragma once
#include "vulkan/VulkanIncludes.h"
#include <string>

namespace celestia {

	class Window
	{
	public:
		Window(int width, int height, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		void setFrameBufferResized(bool val) { framebufferResized = val; }
		bool isFrameBufferResized() { return framebufferResized; }

		void createSurface(VkInstance instance, VkSurfaceKHR *surface);
		void getFramebufferSize(int* width, int* height);

	private:
		void initWindow();
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		const int width;
		const int height;
		const std::string name;
		bool framebufferResized = false;
		GLFWwindow* window = nullptr;
	};

	
}

