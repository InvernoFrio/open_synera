#pragma once

#include "Engine/Math/Ray.h"
#include "Engine/Renderer/Camera.h"

#include <cstdint>

namespace Engine {

    Ray ScreenPointToRay(
        const Camera& camera,
        double mouseX,
        double mouseY,
        uint32_t screenWidth,
        uint32_t screenHeight
    );

}