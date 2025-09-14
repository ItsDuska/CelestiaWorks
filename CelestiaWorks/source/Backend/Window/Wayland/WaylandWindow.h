#pragma once
#include "../PlatformWindow.h"

#ifdef __linux__
#include <wayland-client.h>
// TODO: FIX THIS WHEN ON LINUX

namespace celestia
{
    class WaylandWindow : public PlatformWindow {
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
        bool mouseButton(int buttonIndex, int type) const override;
        int mouseWheel() const override;

        void* getNativeHandle() override { return surface; }
        void* getNativeInstance() override { return display; }

    private:
        wl_display* display;
        wl_surface* surface;
        wl_compositor* compositor;
        wl_shell* shell;
        Vec2i windowSize;
        bool resized;
    };
}
#endif
