#pragma once
#include "System/export.hpp"

#include <memory>
#include <stdint.h>

namespace celestia
{
	struct Font_t;

	/**
	 * @brief Class to load fonts.
	 */
	class Font
	{
	public:
		/**
		 * @brief Basic constructor duh :p
		 */
		CELESTIA_WORKS Font();
		CELESTIA_WORKS Font(const Font&) = delete;
		CELESTIA_WORKS Font& operator=(const Font&) = delete;
		CELESTIA_WORKS ~Font();

		/**
		 * @brief Load a ttf font using a c-like filepath
		 *
		 * @param filepath Filepath to the file relative to the project.
		 * @param fontSize Font size in pixels.
		 * @return Returns true if it succeess.
		 */
		CELESTIA_WORKS bool loadFont(const char* filepath, uint8_t fontSize);

	private:
		std::unique_ptr<Font_t> bitmapData;

		friend class Text;
		friend class WindowHandle;
	};

} // namespace celestia