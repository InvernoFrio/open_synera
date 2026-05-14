#pragma once

#include "Engine/Renderer/RenderItem.h"
#include "Engine/Renderer/SpriteRenderItem.h"
#include "Engine/Renderer/OverlayRenderItem.h"

#include <vector>

namespace Engine {

    class Scene {
    public:
        void Clear();

        void Submit(const RenderItem& item);
        void SubmitSprite(const SpriteRenderItem& item);
        void SubmitOverlay(const OverlayRenderItem& item);

        const std::vector<RenderItem>& GetRenderItems() const;
        const std::vector<SpriteRenderItem>& GetSpriteRenderItems() const;
        const std::vector<OverlayRenderItem>& GetOverlayRenderItems() const;

    private:
        std::vector<RenderItem> m_RenderItems;
        std::vector<SpriteRenderItem> m_SpriteRenderItems;
        std::vector<OverlayRenderItem> m_OverlayRenderItems;
    };

}