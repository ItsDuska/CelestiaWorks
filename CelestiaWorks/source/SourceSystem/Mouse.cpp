#include "System/Mouse.h"
#include "backend/window/WindowContext.h"

CELESTIA_WORKS signed char celestia::Mouse::mouseWheel()
{
    return WindowContext::get()->mouseWheel();
}

CELESTIA_WORKS bool celestia::Mouse::mouseButtonDown(Button button)
{
    return WindowContext::get()->mouseButton(button, 0);
}

CELESTIA_WORKS bool celestia::Mouse::mouseButtonReleased(Button button)
{
    return WindowContext::get()->mouseButton(button, 1);
}

// Returns the position relative to the whole screen.
CELESTIA_WORKS celestia::Vec2i celestia::Mouse::mousePosition()
{ 
    return WindowContext::get()->mousePosition();
}
