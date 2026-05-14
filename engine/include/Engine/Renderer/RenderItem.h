#pragma once

#include "Engine/Renderer/Material.h"

#include <glm/glm.hpp>

#include <cstdint>

namespace Engine {

    enum class MeshType : uint32_t {
        Quad = 0,
        Cube = 1
    };

    struct RenderItem {
        MeshType meshType = MeshType::Quad;

        MaterialId materialId = MaterialId::DebugWhite;

        glm::mat4 model{ 1.0f };
    };

}