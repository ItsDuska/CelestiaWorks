#include "System/Keyboard.h"
#include "backend/window/WindowContext.h"

bool celestia::Keyboard::isKeyPressed(Key key)
{
	return WindowContext::get()->getKeyPressed(static_cast<int>(key));
}
