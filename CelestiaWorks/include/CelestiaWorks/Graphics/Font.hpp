#pragma once
#include "System/export.hpp"

#include <memory>
#include <stdint.h>

namespace celestia
{
    /*
    TODO:
    Font class joka s�il�� datan. V�h�n niinkuin Texture class
    */

    struct Font_t;

    class Font
    {
    public:
        CELESTIA_WORKS Font();
        CELESTIA_WORKS Font(const Font&) = delete;
        CELESTIA_WORKS Font& operator=(const Font&) = delete;
        CELESTIA_WORKS ~Font();

        CELESTIA_WORKS bool loadFont(const char* filepath, uint8_t fontSize);

    private:
        std::unique_ptr<Font_t> bitmapData;

        friend class Text;
        friend class WindowHandle;
    };

} // namespace celestia