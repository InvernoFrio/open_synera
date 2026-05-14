#include "AutoChessGame.h"

#include "Engine/Math/CameraRay.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void AutoChessGame::Init(
    uint32_t width,
    uint32_t height
) {
    constexpr int boardRows = 8;
    constexpr int boardCols = 8;
    constexpr float cellSize = 1.2f;

    m_BoardSystem.CreateBoard(
        boardRows,
        boardCols,
        cellSize
    );

    m_UnitSystem.InitBoardInfo(
        boardRows,
        boardCols,
        cellSize
    );

    m_UnitSystem.SpawnUnit(
        { 1, 2 },
        Engine::MaterialId::UnitBlue,
        1.2f
    );

    m_UnitSystem.SpawnUnit(
        { 5, 5 },
        Engine::MaterialId::UnitRed,
        1.4f
    );

    m_UnitSystem.SpawnUnit(
        { 6, 3 },
        Engine::MaterialId::UnitGreen,
        1.0f
    );

    float aspect =
        static_cast<float>(width) /
        static_cast<float>(height);

    m_Camera.SetPerspective(
        glm::radians(45.0f),
        aspect,
        0.1f,
        100.0f
    );

    m_Camera.LookAt(
        glm::vec3{ 0.0f, 9.5f, 9.5f },
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f }
    );

    BuildScene();
}

void AutoChessGame::Update(
    float dt,
    const Engine::Input& input,
    uint32_t screenWidth,
    uint32_t screenHeight
) {
    if (screenWidth > 0 && screenHeight > 0) {
        Resize(
            screenWidth,
            screenHeight
        );
    }

    HandleMouseInput(
        input,
        screenWidth,
        screenHeight
    );

    m_BoardSystem.Update();
    m_UnitSystem.Update(dt);

    BuildScene();
}

void AutoChessGame::Resize(
    uint32_t width,
    uint32_t height
) {
    if (width == 0 || height == 0) {
        return;
    }

    float aspect =
        static_cast<float>(width) /
        static_cast<float>(height);

    m_Camera.SetAspect(aspect);
}

void AutoChessGame::HandleMouseInput(
    const Engine::Input& input,
    uint32_t screenWidth,
    uint32_t screenHeight
) {
    if (screenWidth == 0 || screenHeight == 0) {
        return;
    }

    Engine::Ray ray =
        Engine::ScreenPointToRay(
            m_Camera,
            input.GetMouseX(),
            input.GetMouseY(),
            screenWidth,
            screenHeight
        );

    auto hitCell =
        m_BoardSystem.RaycastCell(ray);

    if (hitCell.has_value()) {
        m_BoardSystem.HighlightCell(
            hitCell->x,
            hitCell->y
        );
    }
    else {
        m_BoardSystem.ClearHighlight();
        return;
    }

    if (!input.IsMouseButtonPressed(Engine::MouseButton::Left)) {
        return;
    }

    glm::ivec2 cell =
        hitCell.value();

    auto unitAtCell =
        m_UnitSystem.FindUnitAtCell(cell);

    if (unitAtCell.has_value()) {
        m_UnitSystem.SelectUnit(
            unitAtCell.value()
        );

        return;
    }

    if (m_UnitSystem.HasSelection()) {
        m_UnitSystem.MoveSelectedUnitToCell(cell);
    }
}

void AutoChessGame::BuildScene() {
    m_Scene.Clear();

    m_BoardSystem.SubmitRenderItems(
        m_Scene
    );

    m_UnitSystem.SubmitRenderItems(
        m_Scene
    );
}

Engine::Camera& AutoChessGame::GetCamera() {
    return m_Camera;
}

const Engine::Scene& AutoChessGame::GetScene() const {
    return m_Scene;
}