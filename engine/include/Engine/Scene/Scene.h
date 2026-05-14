#pragma once

#include "Engine/Renderer/RenderItem.h"

#include <vector>

namespace Engine {

    class Scene {
    public:
        void Clear();

        void Submit(const RenderItem& item);

        const std::vector<RenderItem>& GetRenderItems() const;

    private:
        std::vector<RenderItem> m_RenderItems;
    };

}