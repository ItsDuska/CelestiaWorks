#pragma once
#include <Windows.h>
#include "backend/vulkanAPI/core/CelestiaVulkanTypes.h"

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
		Vec2i screenSpaceToWindowSpace(Vec2i& position) const;

		static bool getKeyPressed(int key);

		// type means if we are asking for button up or down event.
		static bool mouseButton(signed char buttonIndex, signed char type); 
		static signed char mouseWheel();

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
