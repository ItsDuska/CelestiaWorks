#pragma once
#include "window/Window.h"
#include "Vulkan/Device.h"
#include "vulkan/SwapChain.h"

namespace celestia
{
	class Application
	{
	public:
		Application();
		~Application();

		void run();

	private:
		Window window{ 800,600,"CelestiaWorks" };
		std::shared_ptr<Device> device = std::make_shared<Device>(window);
		SwapChain swapChain{device};
	};

}

