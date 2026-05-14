#pragma once

#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"

#include <unordered_map>
#include <string>

namespace Engine {

    class AssetManager {
    public:
        void RegisterMesh(const std::string& name, const Mesh& mesh);
        void RegisterMaterial(const std::string& name, const Material& material);

        Mesh* GetMesh(const std::string& name);
        Material* GetMaterial(const std::string& name);

    private:
        std::unordered_map<std::string, Mesh> m_Meshes;
        std::unordered_map<std::string, Material> m_Materials;
    };

}