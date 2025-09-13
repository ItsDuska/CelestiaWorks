#pragma once
#include "PlatformWindow.h"


namespace celestia
{
    class WindowContext
    {
    public:
        static void set(PlatformWindow* win) { instance = win; }   
        static PlatformWindow* get() { return instance; }

    private:
        static PlatformWindow* instance;
    };
}
