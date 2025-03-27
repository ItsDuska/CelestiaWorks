#pragma once
#include <memory>
#include "backend/vulkanAPI/core/CelestiaVulkanTypes.h"
#include <chrono>

namespace celestia
{
	class Device;
	class SwapChain;
	class Pipeline;
	class Window;
	class Image;

	class Render
	{
	public:
		Render(Window &window);
		Render(const Render&) = delete;
		Render& operator = (const Render&) = delete;
		~Render();

		void submitIndexedDraw(DrawInfo& info);
		void beginRendering();
		void endRendering();
		void setClearColor(Color& color);
		void cleanUp();
		void setFramerateLimit(const int frameRate);
	private:
		void resize();
		void createCommandBuffers();

	private:
		friend class BatchSpriteRender;
		friend class TextRender;
		Window& window;
		std::unique_ptr<Device> device;
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Image> image;

		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentFrame;
		uint32_t imageIndex;
		bool rendering;
		Vec4 clearColor;
		bool hasBindedTEMP;
		PUSH_CONSTANTS constants;

		std::chrono::system_clock::duration framerateLimit;

		std::chrono::system_clock::time_point beginFrameTime;
		std::chrono::system_clock::time_point endFrameTime;


		//TEMP
		unsigned frame_count_per_second = 0;
		std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> prev_time_in_seconds;
		//TEMP
	};
}
