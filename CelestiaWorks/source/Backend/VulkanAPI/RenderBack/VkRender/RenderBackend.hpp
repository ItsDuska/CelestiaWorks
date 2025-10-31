#pragma once
#include <memory>
#include "Backend/VulkanAPI/Core/CelestiaVulkanTypes.hpp"
#include <chrono>

namespace celestia
{
	class Device;
	class SwapChain;
	class Pipeline;
	class Window;
	class Image;
	class RenderTarget;

	class Render
	{
	public:
		Render();
		Render(const Render&) = delete;
		Render& operator=(const Render&) = delete;
		~Render();

		void submitIndexedDraw(DrawInfo& info);
		void beginRendering();
		void endRendering();
		void setClearColor(Color& color);
		void cleanUp();
		void setFramerateLimit(const int frameRate);
		void setRenderTarget(RenderTarget* target);

	private:
		void resize();
		void createCommandBuffers();
		void createOffscreenCommandBuffer();

	private:
		friend class BatchSpriteRender;
		friend class TextRender;
		friend class DefaultSingleRenderer;

		std::unique_ptr<Device> device;
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Image> image;

		std::vector<VkCommandBuffer> commandBuffers;
		VkCommandBuffer offscreenCommandBuffer; // Separate command buffer for offscreen rendering

		uint32_t currentFrame;
		uint32_t imageIndex;
		bool rendering;
		bool hasBindedTEMP;
		Vec4 clearColor;
		PUSH_CONSTANTS constants;

		RenderTarget* activeRenderTarget = nullptr;
		bool isOffscreenRendering = false; // Track if we're doing offscreen rendering

		std::chrono::system_clock::duration framerateLimit;

		std::chrono::system_clock::time_point beginFrameTime;
		std::chrono::system_clock::time_point endFrameTime;

		std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> prev_time_in_seconds;
		// TEMP
	};
} // namespace celestia
