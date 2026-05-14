#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace Engine {

    enum class OverlayShapeType : uint32_t {
        SolidRect = 0,
        Digit = 1,
        Circle = 2
    };

    struct OverlayRenderItem {
        /*
            原生 framebuffer 像素坐标。
            原点：左上角。
        */
        glm::vec2 position{ 0.0f };

        glm::vec2 size{
            32.0f,
            32.0f
        };

        glm::vec4 color{ 1.0f };

        float rotationRadians = 0.0f;

        /*
            通用形状类型。
            目前先支持 Digit，后续可以扩展：
            - SolidRect
            - Circle
            - Icon
            - TexturedQuad
        */
        OverlayShapeType shapeType =
            OverlayShapeType::SolidRect;

        /*
            params.x:
                Digit 模式下表示 0~9

            params.y:
                outline 开关 / 强度，预留

            params.zw:
                预留
        */
        glm::vec4 params{ 0.0f };
    };

}