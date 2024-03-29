#pragma once
#include <Windows.h>
#include "backend/vulkanAPI/CelestiaVulkanTypes.h"
//#include "System/Keyboard.h"

namespace celestia {
	class Window
	{
	public:
		Window(const Vec2i size, const char* name);
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;
		~Window();
		
		HINSTANCE getInstance();
		HWND getHandle();
		Vec2i getWindowSize() const;

		bool processMessages();
		bool isFramebufferResized() const;
		void setFramebufferResized(bool resize);
		void setWindowSize(Vec2i size);
		static bool getKeyPressed(int key);

	private:
		static void resizeWindow(HWND hWnd, UINT uMsg);
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE hInstance;
		HWND hWnd;
		const wchar_t* CLASS_NAME;
		Vec2i windowSize;
		bool resized;
		
		
	};

}
