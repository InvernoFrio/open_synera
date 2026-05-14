#include "Engine/Renderer/MaterialLibrary.h"

namespace Engine {

    void MaterialLibrary::InitDefaultMaterials() {
        m_FallbackMaterial.id = MaterialId::DebugWhite;
        m_FallbackMaterial.name = "DebugWhite";
        m_FallbackMaterial.baseColor = glm::vec4{
            1.0f,
            1.0f,
            1.0f,
            1.0f
        };
        m_FallbackMaterial.shadeSteps = 3.0f;
        m_FallbackMaterial.shadowStrength = 0.5f;
        m_FallbackMaterial.ambient = 0.35f;

        RegisterMaterial({
            MaterialId::BoardDark,
            "BoardDark",
            glm::vec4{0.18f, 0.24f, 0.32f, 1.0f},
            2.0f,
            0.35f,
            0.50f,
            0.0f,
            0.5f
            });

        RegisterMaterial({
            MaterialId::BoardLight,
            "BoardLight",
            glm::vec4{0.24f, 0.32f, 0.42f, 1.0f},
            2.0f,
            0.35f,
            0.50f,
            0.0f,
            0.5f
            });

        RegisterMaterial({
            MaterialId::BoardHighlight,
            "BoardHighlight",
            glm::vec4{0.95f, 0.75f, 0.20f, 1.0f},
            2.0f,
            0.25f,
            0.55f,
            0.0f,
            0.35f
            });

        RegisterMaterial({
            MaterialId::UnitBlue,
            "UnitBlue",
            glm::vec4{0.25f, 0.55f, 1.0f, 1.0f},
            3.0f,
            0.55f,
            0.30f,
            0.0f,
            0.4f
            });

        RegisterMaterial({
            MaterialId::UnitRed,
            "UnitRed",
            glm::vec4{1.0f, 0.25f, 0.25f, 1.0f},
            3.0f,
            0.55f,
            0.30f,
            0.0f,
            0.4f
            });

        RegisterMaterial({
            MaterialId::UnitGreen,
            "UnitGreen",
            glm::vec4{0.45f, 1.0f, 0.45f, 1.0f},
            3.0f,
            0.55f,
            0.30f,
            0.0f,
            0.4f
            });

        RegisterMaterial({
            MaterialId::UnitSelected,
            "UnitSelected",
            glm::vec4{1.0f, 0.95f, 0.35f, 1.0f},
            3.0f,
            0.35f,
            0.40f,
            0.0f,
            0.25f
            });

        RegisterMaterial({
            MaterialId::OutlineDark,
            "OutlineDark",
            glm::vec4{0.035f, 0.035f, 0.05f, 1.0f},
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f
            });

        RegisterMaterial(m_FallbackMaterial);
    }

    void MaterialLibrary::RegisterMaterial(
        const Material& material
    ) {
        m_Materials[material.id] = material;
    }

    const Material& MaterialLibrary::GetMaterial(
        MaterialId id
    ) const {
        auto it = m_Materials.find(id);

        if (it == m_Materials.end()) {
            return m_FallbackMaterial;
        }

        return it->second;
    }

}