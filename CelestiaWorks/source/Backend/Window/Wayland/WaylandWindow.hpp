#pragma once
#include "../PlatformWindow.hpp"
#include <wayland-client-protocol.h>
#include <wayland-util.h>
#include <unordered_map>

#ifdef __linux__
#include <wayland-client.h>

struct xdg_wm_base;
struct xdg_surface;
struct xdg_toplevel;
struct wl_seat;
struct wl_pointer;
struct wl_keyboard;

namespace celestia
{
	class WaylandWindow : public PlatformWindow
	{
	public:
		WaylandWindow(const Vec2i size, const char* name);
		~WaylandWindow();

		Vec2i getWindowSize() const override;
		bool processMessages() override;
		bool isFramebufferResized() const override;
		void setFramebufferResized(bool resize) override;
		void setWindowSize(Vec2i size) override;
		Vec2i screenSpaceToWindowSpace(Vec2i& position) const override;

		bool getKeyPressed(int key) const override;

		bool mouseButton(signed char buttonIndex, signed char type) const override;
		signed char mouseWheel() const override;
		Vec2i mousePosition() override;

		void* getNativeHandle() override
		{
			return surface;
		}
		void* getNativeInstance() override
		{
			return display;
		}

		static void
		handleRegistry(void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
		static void handleRegistryRemove(void* data, wl_registry* registry, uint32_t name);

		static void handleShellSurfaceConfigure(void* data, struct xdg_surface* shellSurface, uint32_t serial);

		static void
		handleToplevelConfigure(void* data, xdg_toplevel* toplevel, int32_t width, int32_t height, wl_array* states);
		static void handleToplevelClose(void* data, struct xdg_toplevel* toplevel);

		static void handleSeatCapabilities(void* data, wl_seat* seat, uint32_t capabilities);
		static void handleSeatName(void* data, wl_seat* seat, const char* name);

		static void handlePointerEnter(
		  void* data, wl_pointer* pointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y);
		static void handlePointerLeave(void* data, wl_pointer* pointer, uint32_t serial, wl_surface* surface);
		static void handlePointerMotion(void* data, wl_pointer* pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y);
		static void handlePointerButton(
		  void* data, wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
		static void handlePointerAxis(void* data, wl_pointer* pointer, uint32_t time, uint32_t axis, wl_fixed_t value);

		static void handleKeyboardKeymap(void* data, wl_keyboard* keyboard, uint32_t format, int32_t fd, uint32_t size);
		static void
		handleKeyboardEnter(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface, wl_array* keys);
		static void handleKeyboardLeave(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface);
		static void handleKeyboardKey(
		  void* data, wl_keyboard* keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
		static void handleKeyboardModifiers(void* data, wl_keyboard* keyboard, uint32_t serial, uint32_t mods_depressed,
		  uint32_t mods_latched, uint32_t mods_locked, uint32_t group);

	private:
		wl_display* display;
		wl_surface* surface;
		wl_registry* registry;
		wl_compositor* compositor;
		xdg_wm_base* shell;
		xdg_surface* shellSurface;
		xdg_toplevel* toplevel;
		wl_seat* seat;
		wl_pointer* pointer;
		wl_keyboard* keyboard;

		Vec2i windowSize;
		bool resized;
		bool readyToResize;
		bool quit;
		bool hasFocus;

		std::unordered_map<uint32_t, bool> keyStates;
	};
} // namespace celestia
#endif
