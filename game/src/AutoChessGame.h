#pragma once

#include "BoardSystem.h"
#include "UnitSystem.h"

#include "Engine/Input/Input.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Scene/Scene.h"

#include <cstdint>

class AutoChessGame {
public:
    void Init(
        uint32_t width,
        uint32_t height
    );

    void Update(
        float dt,
        const Engine::Input& input,
        uint32_t screenWidth,
        uint32_t screenHeight
    );

    void Resize(
        uint32_t width,
        uint32_t height
    );

    Engine::Camera& GetCamera();

    const Engine::Scene& GetScene() const;

private:
    void HandleMouseInput(
        const Engine::Input& input,
        uint32_t screenWidth,
        uint32_t screenHeight
    );

    void BuildScene();

private:
    BoardSystem m_BoardSystem;
    UnitSystem m_UnitSystem;

    Engine::Camera m_Camera;
    Engine::Scene m_Scene;
};