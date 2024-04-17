#pragma once
#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include <memory>
#include <stdint.h>

namespace celestia
{
	/*
	TODO:
	Font class joka säilöö datan. Vähän niinkuin Texture class
	*/

	struct Font_t;

	class Font
	{
	public:
		CELESTIA_WORKS Font();
		CELESTIA_WORKS Font(const Font&) = delete;
		CELESTIA_WORKS Font& operator = (const Font&) = delete;
		CELESTIA_WORKS ~Font();

		CELESTIA_WORKS bool loadFont(const char* filepath, uint8_t fontSize);
	private:
		std::unique_ptr<Font_t> bitmapData;

		friend class Text;
		friend class WindowHandle;
	};


}