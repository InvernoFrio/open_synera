#pragma once

#include <cstdint>

namespace Engine {

    struct PixelRenderConfig {
        uint32_t internalWidth = 426;
        uint32_t internalHeight = 240;

        bool nearestUpscale = true;
    };

}