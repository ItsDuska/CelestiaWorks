#pragma once
#include "System/Vector.hpp"

namespace celestia
{
	struct InternalMouseStorage
	{
		bool buttons[10];		 // mouse buttons. Each one has 2 states. Down or released
		signed char scrollCount; // How much the scrollwheel has spun. Zero is default state.
		Vec2i mousePosition;	 // Mouse position duh
	};

	static InternalMouseStorage internalMouseStorage{};

	enum ButtonInternal
	{
		RIGHT_CLICK_DOWN,
		RIGHT_CLICK_RELEASED,
		LEFT_CLICK_DOWN,
		LEFT_CLICK_RELEASED,
		MIDDLE_CLICK_DOWN,
		MIDDLE_CLICK_RELEASED,
		SIDE_BUTTON_ONE_DOWN,
		SIDE_BUTTON_ONE_RELEASED,
		SIDE_BUTTON_TWO_DOWN,
		SIDE_BUTTON_TWO_RELEASED
	};
} // namespace celestia
