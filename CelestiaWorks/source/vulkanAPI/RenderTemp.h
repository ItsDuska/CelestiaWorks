#pragma once

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include "window/Window.h"
#include <memory>



namespace celestia {

	class Device;
	class SwapChain;

	class CELESTIA_WORKS Render
	{
	public:
		Render(Window &window);
		~Render();
		void draw();
	private:
		std::unique_ptr<Device> device;
		std::unique_ptr<SwapChain> swapChain;
		//Window window;
	};

}
