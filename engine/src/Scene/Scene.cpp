#include "Engine/Scene/Scene.h"

namespace Engine {

    void Scene::Clear() {
        m_RenderItems.clear();
    }

    void Scene::Submit(const RenderItem& item) {
        m_RenderItems.push_back(item);
    }

    const std::vector<RenderItem>& Scene::GetRenderItems() const {
        return m_RenderItems;
    }

}