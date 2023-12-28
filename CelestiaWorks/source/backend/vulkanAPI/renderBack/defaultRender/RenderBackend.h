#pragma once
#include <memory>
#include "celestiaTypes/CelestiaVulkanTypes.h"

namespace celestia
{
	class Device;
	class SwapChain;
	class Pipeline;
	class Buffer;
	class Window;
	class Image;
	class Descriptor;

	class Render
	{
	public:
		Render(Window &window);
		Render(const Render&) = delete;
		Render& operator = (const Render&) = delete;
		virtual ~Render();

		void draw(const Mesh& mesh,const int amountToDraw);
		void beginRendering();
		void endRendering();
		void setClearColor(Color& color);
		void cleanUp();
	private:
		void resize();
		void createCommandBuffers();

	protected:
		Window& window;
		std::unique_ptr<Device> device;
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Buffer> buffer;
		std::unique_ptr<Image> image;
		std::unique_ptr<Descriptor> descriptor;

		std::unique_ptr<Pipeline> pipeline;

		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentFrame;
		uint32_t imageIndex;
		bool rendering;
		Vec4 clearColor;
		bool hasBindedTEMP;
		
		PUSH_CONSTANTS constants;
	};
}
