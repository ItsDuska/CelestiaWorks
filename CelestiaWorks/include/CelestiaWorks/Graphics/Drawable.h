#pragma once
#include "RenderPipeline.h"

namespace celestia
{
    class RendererHandler;

    class Drawable
    {
    public:
        virtual ~Drawable() = default;
        virtual void draw(const RendererHandler& renderer, RenderPipeline* pipeline) const = 0;
    };
}
