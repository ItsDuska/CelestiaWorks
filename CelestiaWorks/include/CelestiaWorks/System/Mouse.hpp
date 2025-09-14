#pragma once
#include "Vector.hpp"

namespace celestia
{
	namespace Mouse
	{
		enum Button : signed char
		{
			RIGHT_CLICK,
			LEFT_CLICK,
			MIDDLE_CLICK,
			SIDE_BUTTON_ONE,
			SIDE_BUTTON_TWO
		};

		// Returns how much the scroll wheel has rotated between the values ​​ -120 and 120. A negative number is
		// downward and a positive number is upward.
		CELESTIA_WORKS signed char mouseWheel();

		// Returns if given mouse button is down.
		CELESTIA_WORKS bool mouseButtonDown(Button button);

		// Returns if given mouse button has been released.
		CELESTIA_WORKS bool mouseButtonReleased(Button button);

		CELESTIA_WORKS Vec2i mousePosition();
	} // namespace Mouse
} // namespace celestia