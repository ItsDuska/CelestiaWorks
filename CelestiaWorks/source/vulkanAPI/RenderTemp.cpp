#include "RenderTemp.h"
#include "vulkanAPI/Device.h"
#include "vulkanAPI/SwapChain.h"
#include "Pipeline.h"

celestia::Render::Render(Window& window)
{
	device = std::make_unique<Device>(window);
	swapChain = std::make_unique<SwapChain>(*device, window);
	pipeline = std::make_unique<Pipeline>(*device, *swapChain);

	device->print();
}

celestia::Render::~Render()
{
}

void celestia::Render::draw()
{
	std::cout << "Creating SwapChain Again\n";
	swapChain->recreateSwapChain();
	std::cout << "SwapChain created\n";
}
