#pragma once

#include "BoardSystem.h"
#include "UnitSystem.h"

#include "Engine/Input/Input.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/PixelRenderConfig.h"
#include "Engine/Scene/Scene.h"

#include <cstdint>

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

    void BuildScene();

private:
    Engine::PixelRenderConfig m_PixelConfig;

    BoardSystem m_BoardSystem;
    UnitSystem m_UnitSystem;

    Engine::Camera m_Camera;
    Engine::Scene m_Scene;
};