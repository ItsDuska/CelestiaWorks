#pragma once

namespace celestia
{
    class RendererHandler;

    class Drawable
    {
    public:
        virtual ~Drawable() = default;
        virtual void draw(const RendererHandler& renderer) const = 0;
    };
}
