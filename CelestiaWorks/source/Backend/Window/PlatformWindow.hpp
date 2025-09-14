#pragma once
#include "Backend/VulkanAPI/Core/CelestiaVulkanTypes.hpp"

namespace celestia
{
	class PlatformWindow
	{
	public:
		virtual ~PlatformWindow() = default;

		virtual Vec2i getWindowSize() const = 0;
		virtual bool processMessages() = 0;
		virtual bool isFramebufferResized() const = 0;
		virtual void setFramebufferResized(bool resize) = 0;
		virtual void setWindowSize(Vec2i size) = 0;
		virtual Vec2i screenSpaceToWindowSpace(Vec2i& position) const = 0;

		virtual bool getKeyPressed(int key) const = 0;
		virtual bool mouseButton(signed char buttonIndex, signed char type) const = 0;
		virtual signed char mouseWheel() const = 0;
		virtual Vec2i mousePosition() = 0;

		virtual void* getNativeHandle() = 0;
		virtual void* getNativeInstance() = 0;
	};

} // namespace celestia
