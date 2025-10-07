#include "WaylandWindow.hpp"
#include <cstddef>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <linux/input-event-codes.h>

#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

#include "../InternalMouseStorage.hpp"

#define CHECK_WL_RESULT(_expr)                                                                                         \
	if(!(_expr))                                                                                                       \
	{                                                                                                                  \
		printf("Error executing %s.\n", #_expr);                                                                       \
	}

namespace celestia
{
	static void handleShellPing(void* data, struct xdg_wm_base* shell, uint32_t serial)
	{
		xdg_wm_base_pong(shell, serial);
	}

	void WaylandWindow::handleRegistry(
	  void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);

		if(strcmp(interface, wl_compositor_interface.name) == 0)
		{
			window->compositor =
			  static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
		}
		else if(strcmp(interface, xdg_wm_base_interface.name) == 0)
		{
			window->shell = static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
		}
		else if(strcmp(interface, wl_seat_interface.name) == 0)
		{
			window->seat = static_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, 1));
		}
	}

	void WaylandWindow::handleRegistryRemove(void* data, wl_registry* registry, uint32_t name)
	{
	}

	void WaylandWindow::handleShellSurfaceConfigure(void* data, struct xdg_surface* shellSurface, uint32_t serial)
	{
		xdg_surface_ack_configure(shellSurface, serial);
		WaylandWindow* window = static_cast<WaylandWindow*>(data);

		if(window->resized)
		{
			window->readyToResize = true;
		}
	}

	void WaylandWindow::handleToplevelConfigure(
	  void* data, xdg_toplevel* toplevel, int32_t width, int32_t height, wl_array* states)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);

		if(width > 0 && height > 0)
		{
			window->resized = true;
			window->windowSize.x = width;
			window->windowSize.y = height;
		}
	}

	void WaylandWindow::handleToplevelClose(void* data, struct xdg_toplevel* toplevel)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);
		window->quit = true;
	}

	void WaylandWindow::handleSeatCapabilities(void* data, wl_seat* seat, uint32_t capabilities)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);

		if(capabilities & WL_SEAT_CAPABILITY_POINTER)
		{
			if(!window->pointer)
			{
				window->pointer = wl_seat_get_pointer(seat);
				static const struct wl_pointer_listener pointerListener = {
				  WaylandWindow::handlePointerEnter, WaylandWindow::handlePointerLeave,
				  WaylandWindow::handlePointerMotion, WaylandWindow::handlePointerButton,
				  WaylandWindow::handlePointerAxis,
				  nullptr, // frame
				  nullptr, // axis_source
				  nullptr, // axis_stop
				  nullptr, // axis_discrete
				  nullptr, // axis_value120
				  nullptr  // axis_relative_direction
				};
				wl_pointer_add_listener(window->pointer, &pointerListener, window);
			}
		}
		else if(window->pointer)
		{
			wl_pointer_destroy(window->pointer);
			window->pointer = nullptr;
		}

		if(capabilities & WL_SEAT_CAPABILITY_KEYBOARD)
		{
			if(!window->keyboard)
			{
				window->keyboard = wl_seat_get_keyboard(seat);
				static const struct wl_keyboard_listener keyboardListener = {
				  WaylandWindow::handleKeyboardKeymap, WaylandWindow::handleKeyboardEnter,
				  WaylandWindow::handleKeyboardLeave, WaylandWindow::handleKeyboardKey,
				  WaylandWindow::handleKeyboardModifiers,
				  nullptr // repeat_info
				};
				wl_keyboard_add_listener(window->keyboard, &keyboardListener, window);
			}
		}
		else if(window->keyboard)
		{
			wl_keyboard_destroy(window->keyboard);
			window->keyboard = nullptr;
		}
	}

	void WaylandWindow::handleSeatName(void* data, wl_seat* seat, const char* name)
	{
	}

	void WaylandWindow::handlePointerEnter(
	  void* data, wl_pointer* pointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);
		window->hasFocus = true;

		internalMouseStorage.mousePosition.x = wl_fixed_to_int(x);
		internalMouseStorage.mousePosition.y = wl_fixed_to_int(y);
	}

	void WaylandWindow::handlePointerLeave(void* data, wl_pointer* pointer, uint32_t serial, wl_surface* surface)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);
		window->hasFocus = false;
	}

	void WaylandWindow::handlePointerMotion(void* data, wl_pointer* pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
	{
		internalMouseStorage.mousePosition.x = wl_fixed_to_int(x);
		internalMouseStorage.mousePosition.y = wl_fixed_to_int(y);
	}

	void WaylandWindow::handlePointerButton(
	  void* data, wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
	{
		bool pressed = (state == WL_POINTER_BUTTON_STATE_PRESSED);

		switch(button)
		{
		case BTN_LEFT:
			internalMouseStorage
			  .buttons[pressed ? ButtonInternal::LEFT_CLICK_DOWN : ButtonInternal::LEFT_CLICK_RELEASED] = true;
			break;
		case BTN_RIGHT:
			internalMouseStorage
			  .buttons[pressed ? ButtonInternal::RIGHT_CLICK_DOWN : ButtonInternal::RIGHT_CLICK_RELEASED] = true;
			break;
		case BTN_MIDDLE:
			internalMouseStorage
			  .buttons[pressed ? ButtonInternal::MIDDLE_CLICK_DOWN : ButtonInternal::MIDDLE_CLICK_RELEASED] = true;
			break;
		case BTN_SIDE:
			internalMouseStorage
			  .buttons[pressed ? ButtonInternal::SIDE_BUTTON_ONE_DOWN : ButtonInternal::SIDE_BUTTON_ONE_RELEASED] =
			  true;
			break;
		case BTN_EXTRA:
			internalMouseStorage
			  .buttons[pressed ? ButtonInternal::SIDE_BUTTON_TWO_DOWN : ButtonInternal::SIDE_BUTTON_TWO_RELEASED] =
			  true;
			break;
		}
	}

	void
	WaylandWindow::handlePointerAxis(void* data, wl_pointer* pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
	{
		if(axis == WL_POINTER_AXIS_VERTICAL_SCROLL)
		{
			internalMouseStorage.scrollCount = wl_fixed_to_int(value);
		}
	}

	void
	WaylandWindow::handleKeyboardKeymap(void* data, wl_keyboard* keyboard, uint32_t format, int32_t fd, uint32_t size)
	{
		close(fd);
	}

	void WaylandWindow::handleKeyboardEnter(
	  void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface, wl_array* keys)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);
		window->hasFocus = true;
	}

	void WaylandWindow::handleKeyboardLeave(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);
		window->hasFocus = false;
	}

	void WaylandWindow::handleKeyboardKey(
	  void* data, wl_keyboard* keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
	{
		WaylandWindow* window = static_cast<WaylandWindow*>(data);
		bool pressed = (state == WL_KEYBOARD_KEY_STATE_PRESSED);
		window->keyStates[key] = pressed;
	}

	void WaylandWindow::handleKeyboardModifiers(void* data, wl_keyboard* keyboard, uint32_t serial,
	  uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
	{
	}

	static const struct wl_registry_listener registryListener = {
	  WaylandWindow::handleRegistry, WaylandWindow::handleRegistryRemove};

	static const struct xdg_wm_base_listener shellListener = {handleShellPing};

	static const struct xdg_surface_listener shellSurfaceListener = {WaylandWindow::handleShellSurfaceConfigure};

	static const struct xdg_toplevel_listener toplevelListener = {
	  WaylandWindow::handleToplevelConfigure, WaylandWindow::handleToplevelClose, nullptr, nullptr};

	static const struct wl_seat_listener seatListener = {
	  WaylandWindow::handleSeatCapabilities, WaylandWindow::handleSeatName};

	// Constructor
	WaylandWindow::WaylandWindow(const Vec2i size, const char* name)
		: display(nullptr), surface(nullptr), registry(nullptr), compositor(nullptr), shell(nullptr),
		  shellSurface(nullptr), toplevel(nullptr), seat(nullptr), pointer(nullptr), keyboard(nullptr),
		  windowSize(size), resized(false), readyToResize(false), quit(false), hasFocus(false)
	{
		display = wl_display_connect(nullptr);
		if(!display)
		{
			std::cerr << "ERROR: Failed to connect to Wayland display!\n";
			return;
		}

		registry = wl_display_get_registry(display);
		if(!registry)
		{
			std::cerr << "ERROR: Failed to get Wayland registry!\n";
			wl_display_disconnect(display);
			display = nullptr;
			return;
		}

		wl_registry_add_listener(registry, &registryListener, this);
		wl_display_roundtrip(display);

		if(!compositor || !shell)
		{
			std::cerr << "ERROR: Failed to get required Wayland globals!\n";
			if(registry)
			{
				wl_registry_destroy(registry);
			}
			if(display)
			{
				wl_display_disconnect(display);
			}
			display = nullptr;
			return;
		}

		xdg_wm_base_add_listener(shell, &shellListener, this);

		surface = wl_compositor_create_surface(compositor);
		if(!surface)
		{
			std::cerr << "ERROR: Failed to create Wayland surface!\n";
			return;
		}

		shellSurface = xdg_wm_base_get_xdg_surface(shell, surface);
		if(!shellSurface)
		{
			std::cerr << "ERROR: Failed to create shell surface!\n";
			return;
		}
		xdg_surface_add_listener(shellSurface, &shellSurfaceListener, this);

		toplevel = xdg_surface_get_toplevel(shellSurface);
		if(!toplevel)
		{
			std::cerr << "ERROR: Failed to create toplevel!\n";
			return;
		}
		xdg_toplevel_add_listener(toplevel, &toplevelListener, this);

		xdg_toplevel_set_title(toplevel, name);
		xdg_toplevel_set_app_id(toplevel, name);

		if(seat)
		{
			wl_seat_add_listener(seat, &seatListener, this);
		}

		wl_surface_commit(surface);
		wl_display_roundtrip(display);

		initialised = true;
	}

	WaylandWindow::~WaylandWindow()
	{
		if(pointer)
			wl_pointer_destroy(pointer);
		if(keyboard)
			wl_keyboard_destroy(keyboard);
		if(seat)
			wl_seat_destroy(seat);
		if(toplevel)
			xdg_toplevel_destroy(toplevel);
		if(shellSurface)
			xdg_surface_destroy(shellSurface);
		if(surface)
			wl_surface_destroy(surface);
		if(shell)
			xdg_wm_base_destroy(shell);
		if(compositor)
			wl_compositor_destroy(compositor);
		if(registry)
			wl_registry_destroy(registry);
		if(display)
			wl_display_disconnect(display);
	}

	Vec2i WaylandWindow::getWindowSize() const
	{
		return windowSize;
	}

	bool WaylandWindow::processMessages()
	{
		if(!display)
			return false;

		// Clear mouse button states at the start of each frame
		for(auto& button : internalMouseStorage.buttons)
		{
			button = false;
		}
		internalMouseStorage.scrollCount = 0;

		// Process pending events
		while(wl_display_prepare_read(display) != 0)
		{
			wl_display_dispatch_pending(display);
		}

		if(wl_display_flush(display) < 0)
		{
			wl_display_cancel_read(display);
			return false;
		}

		if(wl_display_read_events(display) < 0)
		{
			return false;
		}

		wl_display_dispatch_pending(display);

		return !quit;
	}

	bool WaylandWindow::isFramebufferResized() const
	{
		return resized;
	}

	void WaylandWindow::setFramebufferResized(bool resize)
	{
		resized = resize;
	}

	void WaylandWindow::setWindowSize(Vec2i size)
	{
		windowSize = size;
	}

	Vec2i WaylandWindow::screenSpaceToWindowSpace(Vec2i& position) const
	{
		return position;
	}

	bool WaylandWindow::getKeyPressed(int key) const
	{
		if(!hasFocus)
			return false;

		// Map Windows virtual key codes to Linux keycodes
		uint32_t linuxKey = 0;
		switch(key)
		{
		case 0x41: linuxKey = KEY_A; break;
		case 0x42: linuxKey = KEY_B; break;
		case 0x43: linuxKey = KEY_C; break;
		case 0x44: linuxKey = KEY_D; break;
		case 0x45: linuxKey = KEY_E; break;
		case 0x46: linuxKey = KEY_F; break;
		case 0x47: linuxKey = KEY_G; break;
		case 0x48: linuxKey = KEY_H; break;
		case 0x49: linuxKey = KEY_I; break;
		case 0x4A: linuxKey = KEY_J; break;
		case 0x4B: linuxKey = KEY_K; break;
		case 0x4C: linuxKey = KEY_L; break;
		case 0x4D: linuxKey = KEY_M; break;
		case 0x4E: linuxKey = KEY_N; break;
		case 0x4F: linuxKey = KEY_O; break;
		case 0x50: linuxKey = KEY_P; break;
		case 0x51: linuxKey = KEY_Q; break;
		case 0x52: linuxKey = KEY_R; break;
		case 0x53: linuxKey = KEY_S; break;
		case 0x54: linuxKey = KEY_T; break;
		case 0x55: linuxKey = KEY_U; break;
		case 0x56: linuxKey = KEY_V; break;
		case 0x57: linuxKey = KEY_W; break;
		case 0x58: linuxKey = KEY_X; break;
		case 0x59: linuxKey = KEY_Y; break;
		case 0x5A: linuxKey = KEY_Z; break;
		case 0x20: linuxKey = KEY_SPACE; break;
		case 0x0D: linuxKey = KEY_ENTER; break;
		case 0x1B: linuxKey = KEY_ESC; break;
		default: return false;
		}

		auto it = keyStates.find(linuxKey);
		return (it != keyStates.end()) ? it->second : false;
	}

	bool WaylandWindow::mouseButton(signed char buttonIndex, signed char type) const
	{
		int index = buttonIndex * 2 + type;
		if(index >= 0 && index < 10)
		{
			return internalMouseStorage.buttons[index];
		}
		return false;
	}

	signed char WaylandWindow::mouseWheel() const
	{
		return internalMouseStorage.scrollCount;
	}

	Vec2i WaylandWindow::mousePosition()
	{
		return internalMouseStorage.mousePosition;
	}

} // namespace celestia
