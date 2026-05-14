#pragma once

#include "Engine/Renderer/Material.h"

#include <unordered_map>

namespace Engine {

    class MaterialLibrary {
    public:
        void InitDefaultMaterials();

        void RegisterMaterial(
            const Material& material
        );

        const Material& GetMaterial(
            MaterialId id
        ) const;

    private:
        std::unordered_map<MaterialId, Material> m_Materials;
        Material m_FallbackMaterial;
    };

}