#pragma once

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include <vector>
#include <memory>
#include <vulkan/vulkan.h>

namespace celestia
{
	class Device;
	class SwapChain;
	class Pipeline;
	class Buffer;
	class Window;

	class CELESTIA_WORKS Render
	{
	public:
		Render(Window &window);
		Render(const Render&) = delete;
		Render& operator = (const Render&) = delete;
		~Render();

		void draw();
		void beginRendering();
		void endRendering();
	private:
		void resize();
		void createCommandBuffers();

	private:
		Window& window;
		std::unique_ptr<Device> device;
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		std::unique_ptr<Buffer> buffer;

		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentFrame;
		uint32_t imageIndex;
		bool rendering;
		//Window window;
		


	};
}
