#pragma once

#include "Engine/Renderer/RenderItem.h"
#include "Engine/Renderer/SpriteRenderItem.h"

#include <vector>

namespace Engine {

    class Scene {
    public:
        void Clear();

        void Submit(const RenderItem& item);
        void SubmitSprite(const SpriteRenderItem& item);

        const std::vector<RenderItem>& GetRenderItems() const;
        const std::vector<SpriteRenderItem>& GetSpriteRenderItems() const;

    private:
        std::vector<RenderItem> m_RenderItems;
        std::vector<SpriteRenderItem> m_SpriteRenderItems;
    };

}