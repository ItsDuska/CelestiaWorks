#include "Window.h"

celestia::Window::Window(int width, int height, std::string name)
	: width{ width }, height{ height }, name{ name }
{
	initWindow();
}

celestia::Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void celestia::Window::createSurface(VkInstance instance, VkSurfaceKHR *surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface))
	{
		throw std::runtime_error("Failed to create window surface!");
	}
}

void celestia::Window::getFramebufferSize(int* width, int* height)
{
	glfwGetFramebufferSize(window, width, height);
}

void celestia::Window::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void celestia::Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}
