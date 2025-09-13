#pragma once
#include "../PlatformWindow.h"

#ifdef _WIN32
#include <Windows.h>

namespace celestia {
	class Win32Window : public PlatformWindow
	{
	public:
		Win32Window(const Vec2i size, const char* name);
		Win32Window(const Win32Window&) = delete;
		Win32Window& operator = (const Win32Window&) = delete;
		~Win32Window();
		
		void* getNativeInstance() override;
		void* getNativeHandle() override;
		Vec2i getWindowSize() const override;

		bool processMessages() override;
		bool isFramebufferResized() const override;
		void setFramebufferResized(bool resize) override;
		void setWindowSize(Vec2i size) override;
		Vec2i screenSpaceToWindowSpace(Vec2i& position) const override;

		bool getKeyPressed(int key) const override;

		// type means if we are asking for button up or down event.
		bool mouseButton(signed char buttonIndex, signed char type) const override; 
		signed char mouseWheel() const override;
		Vec2i mousePosition() override;

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
#endif
