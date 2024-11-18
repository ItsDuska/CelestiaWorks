#include "System/Mouse.h"
#include "backend/window/Window.h"

CELESTIA_WORKS signed char celestia::Mouse::mouseWheel()
{
    return Window::mouseWheel();
}

CELESTIA_WORKS bool celestia::Mouse::mouseButtonDown(Button button)
{
    return Window::mouseButton(button,0);
}

CELESTIA_WORKS bool celestia::Mouse::mouseButtonReleased(Button button)
{
    return Window::mouseButton(button, 1);
}

// Returns the position relative to the whole screen.
CELESTIA_WORKS celestia::Vec2i celestia::Mouse::mousePosition()
{
    POINT point;
    GetCursorPos(&point);
    return Vec2i{ static_cast<int32_t>(point.x), static_cast<int32_t>(point.y) };
    
}
