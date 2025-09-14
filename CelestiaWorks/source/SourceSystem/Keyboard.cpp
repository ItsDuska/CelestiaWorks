#include "System/Keyboard.hpp"
#include "Backend/Window/WindowContext.hpp"

bool celestia::Keyboard::isKeyPressed(Key key)
{
	return WindowContext::get()->getKeyPressed(static_cast<int>(key));
}
