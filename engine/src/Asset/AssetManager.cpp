#include "Engine/Asset/AssetManager.h"

namespace Engine {

    void AssetManager::RegisterMesh(const std::string& name, const Mesh& mesh) {
        m_Meshes[name] = mesh;
    }

    void AssetManager::RegisterMaterial(const std::string& name, const Material& material) {
        m_Materials[name] = material;
    }

    Mesh* AssetManager::GetMesh(const std::string& name) {
        auto it = m_Meshes.find(name);

        if (it == m_Meshes.end()) {
            return nullptr;
        }

        return &it->second;
    }

    Material* AssetManager::GetMaterial(const std::string& name) {
        auto it = m_Materials.find(name);

        if (it == m_Materials.end()) {
            return nullptr;
        }

        return &it->second;
    }

}