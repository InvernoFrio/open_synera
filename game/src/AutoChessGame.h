#pragma once

#include "BoardSystem.h"
#include "BoardVisualLayer.h"
#include "UnitSystem.h"
#include "EffectVisualLayer.h"

#include "Engine/Input/Input.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/PixelRenderConfig.h"
#include "Engine/Scene/Scene.h"

#include <cstdint>
#include <optional>
#include <vector>

class AutoChessGame {
public:
    void Init(
        const Engine::PixelRenderConfig& pixelConfig
    );

    void Update(
        float dt,
        const Engine::Input& input,
        uint32_t framebufferWidth,
        uint32_t framebufferHeight
    );

    Engine::Camera& GetCamera();

    const Engine::Scene& GetScene() const;

private:
    void HandleMouseInput(
        const Engine::Input& input,
        uint32_t framebufferWidth,
        uint32_t framebufferHeight
    );

    void UpdateBoardVisuals(
        std::optional<glm::ivec2> hoverCell
    );

    void BuildScene();

private:
    Engine::PixelRenderConfig m_PixelConfig;

    BoardSystem m_BoardSystem;
    BoardVisualLayer m_BoardVisualLayer;

    UnitSystem m_UnitSystem;
    EffectVisualLayer m_EffectVisualLayer;

    Engine::Camera m_Camera;
    Engine::Scene m_Scene;
};