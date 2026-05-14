#include "AutoChessGame.h"

#include "Engine/Math/CameraRay.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void AutoChessGame::Init(
    const Engine::PixelRenderConfig& pixelConfig
) {
    m_PixelConfig = pixelConfig;

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

    /*
        关键修复：
        Camera aspect 必须使用内部像素渲染分辨率，
        而不是窗口 framebuffer 分辨率。

        因为真正的 3D scene 是渲染到 offscreen 426x240，
        最后才 letterbox 放大到窗口。
    */
    float aspect =
        Engine::GetPixelInternalAspect(
            m_PixelConfig
        );

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
    uint32_t framebufferWidth,
    uint32_t framebufferHeight
) {
    HandleMouseInput(
        input,
        framebufferWidth,
        framebufferHeight
    );

    m_BoardSystem.Update();
    m_UnitSystem.Update(dt);

    BuildScene();
}

void AutoChessGame::HandleMouseInput(
    const Engine::Input& input,
    uint32_t framebufferWidth,
    uint32_t framebufferHeight
) {
    if (framebufferWidth == 0 ||
        framebufferHeight == 0) {
        return;
    }

    Engine::PixelViewport viewport =
        Engine::CalculatePixelViewport(
            m_PixelConfig,
            framebufferWidth,
            framebufferHeight
        );

    if (!viewport.IsValid()) {
        m_BoardSystem.ClearHighlight();
        return;
    }

    double mouseX =
        input.GetMouseX();

    double mouseY =
        input.GetMouseY();

    bool insidePixelViewport =
        mouseX >= static_cast<double>(viewport.x) &&
        mouseY >= static_cast<double>(viewport.y) &&
        mouseX < static_cast<double>(viewport.x + static_cast<int32_t>(viewport.width)) &&
        mouseY < static_cast<double>(viewport.y + static_cast<int32_t>(viewport.height));

    if (!insidePixelViewport) {
        m_BoardSystem.ClearHighlight();
        return;
    }

    double localMouseX =
        mouseX - static_cast<double>(viewport.x);

    double localMouseY =
        mouseY - static_cast<double>(viewport.y);

    /*
        注意：
        这里用 viewport.width / viewport.height 是正确的。
        因为鼠标是在最终显示区域内定位的。

        camera aspect 使用的是内部 426/240。
        viewport 的宽高也保持同样比例。
    */
    Engine::Ray ray =
        Engine::ScreenPointToRay(
            m_Camera,
            localMouseX,
            localMouseY,
            viewport.width,
            viewport.height
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