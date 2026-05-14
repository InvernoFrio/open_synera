#pragma once

#include <glm/glm.hpp>

namespace Engine {

    struct SpriteRenderItem {
        glm::vec3 worldPosition{ 0.0f };

        glm::vec2 size{
            0.4f,
            0.4f
        };

        glm::vec4 color{ 1.0f };

        float rotationRadians = 0.0f;

        /*
            0 = 普通 billboard
            之后可以扩展：
            1 = additive
            2 = damage number
            3 = icon
        */
        float spriteType = 0.0f;
    };

}