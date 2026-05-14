#include "Engine/Scene/Scene.h"

namespace Engine {

    void Scene::Clear() {
        m_RenderItems.clear();
        m_SpriteRenderItems.clear();
        m_OverlayRenderItems.clear();
    }

    void Scene::Submit(
        const RenderItem& item
    ) {
        m_RenderItems.push_back(item);
    }

    void Scene::SubmitSprite(
        const SpriteRenderItem& item
    ) {
        m_SpriteRenderItems.push_back(item);
    }

    void Scene::SubmitOverlay(
        const OverlayRenderItem& item
    ) {
        m_OverlayRenderItems.push_back(item);
    }

    const std::vector<RenderItem>& Scene::GetRenderItems() const {
        return m_RenderItems;
    }

    const std::vector<SpriteRenderItem>& Scene::GetSpriteRenderItems() const {
        return m_SpriteRenderItems;
    }

    const std::vector<OverlayRenderItem>& Scene::GetOverlayRenderItems() const {
        return m_OverlayRenderItems;
    }

}