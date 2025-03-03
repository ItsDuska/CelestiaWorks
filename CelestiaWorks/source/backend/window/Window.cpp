#include "Window.h"
#include <vector>
#include <iostream>

namespace celestia
{
	static bool focus = true;

	struct InternalMouseStorage
	{
		bool buttons[10]; // mouse buttons. Each one has 2 states. Down or released
		signed char scrollCount; // How much the scrollwheel has spun. Zero is default state.
	};

	static InternalMouseStorage internalMouseStorage{};


	enum ButtonInternal
	{
		RIGHT_CLICK_DOWN,
		RIGHT_CLICK_RELEASED,
		LEFT_CLICK_DOWN,
		LEFT_CLICK_RELEASED,
		MIDDLE_CLICK_DOWN,
		MIDDLE_CLICK_RELEASED,
		SIDE_BUTTON_ONE_DOWN,
		SIDE_BUTTON_ONE_RELEASED,
		SIDE_BUTTON_TWO_DOWN,
		SIDE_BUTTON_TWO_RELEASED
	};

	LRESULT CALLBACK Window::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ERASEBKGND:
			return 1;
		case WM_KILLFOCUS:
			focus = false;
			break;
		case WM_SETFOCUS:
			focus = true;
			break;
		case WM_SIZE:
			resizeWindow(hWnd, WM_SIZE); 
			break;

		case WM_XBUTTONDOWN:
			if (HIWORD(wParam) == XBUTTON1)
			{
				internalMouseStorage.buttons[ButtonInternal::SIDE_BUTTON_ONE_DOWN] = true;
			}
			else if (HIWORD(wParam) == XBUTTON2)
			{
				internalMouseStorage.buttons[ButtonInternal::SIDE_BUTTON_TWO_DOWN] = true;
			}
			return 0;

		case WM_XBUTTONUP:
			if (HIWORD(wParam) == XBUTTON1)
			{
				internalMouseStorage.buttons[ButtonInternal::SIDE_BUTTON_ONE_RELEASED] = true;
			}
			else if (HIWORD(wParam) == XBUTTON2)
			{
				internalMouseStorage.buttons[ButtonInternal::SIDE_BUTTON_TWO_RELEASED] = true;
			}
			return 0;


		case WM_MBUTTONDOWN:
			internalMouseStorage.buttons[ButtonInternal::MIDDLE_CLICK_DOWN] = true;
			
			return 0;

		case WM_MBUTTONUP:
			internalMouseStorage.buttons[ButtonInternal::MIDDLE_CLICK_RELEASED] = true;
			return 0;

		case WM_LBUTTONDOWN:
			internalMouseStorage.buttons[ButtonInternal::LEFT_CLICK_DOWN] = true;
			return 0;
		case WM_LBUTTONUP:
			internalMouseStorage.buttons[ButtonInternal::LEFT_CLICK_RELEASED] = true;
			return 0;
		case WM_RBUTTONDOWN:
			internalMouseStorage.buttons[ButtonInternal::RIGHT_CLICK_DOWN] = true;
			return 0;
		case WM_RBUTTONUP:
			internalMouseStorage.buttons[ButtonInternal::RIGHT_CLICK_RELEASED] = true;
			return 0;
		case WM_MOUSEWHEEL:
			internalMouseStorage.scrollCount = GET_WHEEL_DELTA_WPARAM(wParam);
			return 0;
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

// Convert const char* to const wchar_t* for Unicode compatibility
const size_t nameLen = strlen(name) + 1;
std::vector<wchar_t> vec(nameLen);
MultiByteToWideChar(CP_UTF8, 0, name, (int)nameLen, &vec[0], (int)nameLen);
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
		std::memset(&internalMouseStorage, 0, sizeof(InternalMouseStorage));
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

	Vec2i Window::screenSpaceToWindowSpace(Vec2i& position) const
	{
		POINT point = { position.x,position.y };
		ScreenToClient(this->hWnd, &point);
		return Vec2i(point.x,point.y);
	}

	bool Window::getKeyPressed(int key)
	{
		return ((GetKeyState(key) & 0x8000) && focus );
	}

	bool Window::mouseButton(signed char buttonIndex, signed char type)
	{
		return internalMouseStorage.buttons[buttonIndex * 2 + type];
	}

	signed char Window::mouseWheel()
	{
		return internalMouseStorage.scrollCount;
	}

	void Window::resizeWindow(HWND hWnd,UINT uMsg)
	{
		Window *windowPtr = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (!windowPtr)
		{
			std::cout << "Window's pointer is wrong?????\n";
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
