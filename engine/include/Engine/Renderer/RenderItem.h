#pragma once

#include "Engine/Renderer/Material.h"

#include <glm/glm.hpp>

#include <cstdint>

namespace Engine {

    enum class MeshType : uint32_t {
        Quad = 0,
        Cube = 1
    };

    enum class RenderFlags : uint32_t {
        None = 0,
        Outline = 1 << 0
    };

    inline RenderFlags operator|(
        RenderFlags a,
        RenderFlags b
        ) {
        return static_cast<RenderFlags>(
            static_cast<uint32_t>(a) |
            static_cast<uint32_t>(b)
            );
    }

    inline bool HasRenderFlag(
        uint32_t flags,
        RenderFlags flag
    ) {
        return
            (flags & static_cast<uint32_t>(flag)) != 0;
    }

    struct RenderItem {
        MeshType meshType = MeshType::Quad;

        MaterialId materialId = MaterialId::DebugWhite;

        glm::mat4 model{ 1.0f };

        uint32_t flags = 0;

        float outlineWidth = 0.035f;
        MaterialId outlineMaterialId = MaterialId::OutlineDark;
    };

}