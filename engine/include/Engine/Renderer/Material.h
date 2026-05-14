#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <string>

namespace Engine {

    enum class MaterialId : uint32_t {
        None = 0,

        BoardDark,
        BoardLight,
        BoardHighlight,

        UnitBlue,
        UnitRed,
        UnitGreen,
        UnitSelected,

        DebugWhite
    };

    struct Material {
        MaterialId id = MaterialId::None;

        std::string name;

        glm::vec4 baseColor{ 1.0f };

        float metallic = 0.0f;
        float roughness = 0.5f;
    };

}