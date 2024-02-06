#include "Window.h"
#include <vector>
#include <iostream>
#include "SourceGraphics/Keyboard.h"


namespace celestia
{
	LRESULT CALLBACK Window::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ERASEBKGND:
			return 1;
		case WM_KILLFOCUS:
			//focus = false;
			break;
		case WM_SETFOCUS:
			//focus = true;
			break;
		case WM_SIZE:
			resizeWindow(hWnd, WM_SIZE); 
			break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
			/*
			if (focus)
			{
				static bool keyDown, keyWasDown;
				keyDown =    ((lParam & (1 << 31)) == 0); // magic
				keyWasDown = ((lParam & (1 << 30)) != 0); // magic part 2
				if (keyDown != keyWasDown)
				{
					keys[static_cast<uint8_t>(wParam)] = keyDown;
				}
			}
			*/
			break;

		case WM_SIZING:
			resizeWindow(hWnd, WM_SIZING);
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_NCCREATE:
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			Window* windowPtr = (Window*)pcs->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	Window::Window(const Vec2i size, const char* name)
		: hInstance(GetModuleHandle(nullptr)),
		CLASS_NAME(L"Celestia Works Window")
	{
		windowSize = size;
		resized = false;

		//memset(keys, 0, 256 * sizeof(keys[0]));
		//focus = true;

		WNDCLASS wndClass = {};
		wndClass.lpszClassName = CLASS_NAME;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpfnWndProc = windowProc;

		RegisterClass(&wndClass);
		DWORD style = WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_OVERLAPPED;

		RECT rect{};
		rect.left = 250;
		rect.top = 250;
		rect.right = rect.left + size.x;
		rect.bottom = rect.top + size.y;

		AdjustWindowRect(&rect, style, false);

		
		//Change name's type from const char* to const wchar_t*
		//bruh this is goofy as hell...
		const size_t nameLen = strlen(name) + 1;
		std::vector<wchar_t> vec;
		vec.resize(nameLen);
		int needed = MultiByteToWideChar(CP_UTF8, 0, name, (int)nameLen, &vec[0], (int)nameLen);
		const wchar_t* nameWide = &vec[0];

		hWnd = CreateWindowEx(
			0,
			CLASS_NAME,
			nameWide,
			style,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			hInstance,
			this
		);
		
		ShowWindow(hWnd, SW_SHOW);

		

	}

	Window::~Window()
	{
		UnregisterClass(CLASS_NAME, hInstance);
	}

	HINSTANCE Window::getInstance()
	{
		return hInstance;
	}

	HWND Window::getHandle()
	{
		return hWnd;
	}

	Vec2i Window::getWindowSize() const
	{
		return windowSize;
	}

	bool Window::processMessages()
	{
		MSG msg{};
		
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return true;
	}

	bool Window::isFramebufferResized() const
	{
		return resized;
	}

	void Window::setFramebufferResized(bool resize)
	{
		resized = resize;
	}

	void Window::setWindowSize(Vec2i size)
	{
		windowSize = size;
	}

	void Window::resizeWindow(HWND hWnd,UINT uMsg)
	{
		Window *windowPtr = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (!windowPtr)
		{
			std::cout << "SHART?????\n";
			return;
		}

		windowPtr->resized = true;

		if (uMsg == WM_SIZING)
		{
			windowPtr->windowSize = { 0,0 };
			return;
		}
		
		Vec2i newWindowSize{};
		RECT rect;
		if (!GetClientRect(hWnd, &rect))
		{
			std::cout << "ERROR: rezing failed????? wahhh?\n";
			return;
		}

		newWindowSize.x = rect.right - rect.left;
		newWindowSize.y = rect.bottom - rect.top;

		windowPtr->setWindowSize(newWindowSize);
	}
}
