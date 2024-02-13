#include "System/Keyboard.h"
#include "backend/window/Window.h"

bool celestia::Keyboard::isKeyPressed(Key key)
{
	return Window::getKeyPressed(static_cast<int>(key));
}
