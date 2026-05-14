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

    UpdateBoardVisuals(
        std::nullopt
    );

    BuildScene();
}

void AutoChessGame::Update(
    float dt,
    const Engine::Input& input,
    uint32_t framebufferWidth,
    uint32_t framebufferHeight
) {
    m_LastFramebufferWidth =
        framebufferWidth;

    m_LastFramebufferHeight =
        framebufferHeight;

    HandleMouseInput(
        input,
        framebufferWidth,
        framebufferHeight
    );

    m_BoardSystem.Update();
    m_UnitSystem.Update(dt);
    m_EffectVisualLayer.Update(dt);
    m_DamageNumberLayer.Update(dt);

    BuildScene();
}

void AutoChessGame::HandleMouseInput(
    const Engine::Input& input,
    uint32_t framebufferWidth,
    uint32_t framebufferHeight
) {
    if (framebufferWidth == 0 ||
        framebufferHeight == 0) {
        UpdateBoardVisuals(
            std::nullopt
        );

        return;
    }

    Engine::PixelViewport viewport =
        Engine::CalculatePixelViewport(
            m_PixelConfig,
            framebufferWidth,
            framebufferHeight
        );

    if (!viewport.IsValid()) {
        UpdateBoardVisuals(
            std::nullopt
        );

        return;
    }

    double mouseX =
        input.GetMouseX();

    double mouseY =
        input.GetMouseY();

    bool insidePixelViewport =
        mouseX >= static_cast<double>(viewport.x) &&
        mouseY >= static_cast<double>(viewport.y) &&
        mouseX < static_cast<double>(
            viewport.x + static_cast<int32_t>(viewport.width)
            ) &&
        mouseY < static_cast<double>(
            viewport.y + static_cast<int32_t>(viewport.height)
            );

    if (!insidePixelViewport) {
        UpdateBoardVisuals(
            std::nullopt
        );

        return;
    }

    double localMouseX =
        mouseX - static_cast<double>(viewport.x);

    double localMouseY =
        mouseY - static_cast<double>(viewport.y);

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

    UpdateBoardVisuals(
        hitCell
    );

    if (!hitCell.has_value()) {
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

        glm::vec3 effectPosition =
            m_BoardSystem.CellToWorld(
                cell.x,
                cell.y
            );

        effectPosition.y = 1.2f;

        m_EffectVisualLayer.SpawnEffect(
            EffectType::HitSpark,
            effectPosition
        );

        m_DamageNumberLayer.ShowDamage(
            120,
            effectPosition,
            DamageNumberType::Physical
        );

        return;
    }

    if (m_UnitSystem.HasSelection()) {
        m_UnitSystem.MoveSelectedUnitToCell(cell);

        glm::vec3 effectPosition =
            m_BoardSystem.CellToWorld(
                cell.x,
                cell.y
            );

        effectPosition.y = 0.35f;

        m_EffectVisualLayer.SpawnEffect(
            EffectType::MagicSpark,
            effectPosition
        );

        m_DamageNumberLayer.ShowDamage(
            80,
            effectPosition,
            DamageNumberType::Magic
        );
    }
}

void AutoChessGame::UpdateBoardVisuals(
    std::optional<glm::ivec2> hoverCell
) {
    /*
        这一层现在只是图形演示。
        之后你写对局逻辑时，可以把下面这些范围替换成真实数据：
            - 可移动格子
            - 攻击范围
            - 技能范围
            - 不可放置格子
    */

    m_BoardVisualLayer.Clear();

    if (hoverCell.has_value()) {
        m_BoardVisualLayer.SetHoverCell(
            hoverCell
        );
    }

    auto selectedCell =
        m_UnitSystem.GetSelectedUnitCell();

    if (selectedCell.has_value()) {
        m_BoardVisualLayer.SetSelectedCell(
            selectedCell
        );
    }

    /*
        示例：固定展示几个不同类型的视觉格子。
        后续接你的逻辑时，可以删掉这些 demo。
    */
    m_BoardVisualLayer.AddHighlight(
        { 0, 0 },
        BoardHighlightType::MoveTarget
    );

    m_BoardVisualLayer.AddHighlight(
        { 0, 1 },
        BoardHighlightType::MoveTarget
    );

    m_BoardVisualLayer.AddHighlight(
        { 1, 0 },
        BoardHighlightType::MoveTarget
    );

    m_BoardVisualLayer.AddHighlight(
        { 2, 4 },
        BoardHighlightType::AttackRange
    );

    m_BoardVisualLayer.AddHighlight(
        { 2, 5 },
        BoardHighlightType::AttackRange
    );

    m_BoardVisualLayer.AddHighlight(
        { 3, 5 },
        BoardHighlightType::SkillRange
    );

    m_BoardVisualLayer.AddHighlight(
        { 4, 5 },
        BoardHighlightType::SkillRange
    );

    m_BoardVisualLayer.AddHighlight(
        { 7, 7 },
        BoardHighlightType::Invalid
    );

    /*
        选中格演示。
        当前 UnitSystem 暂时没有暴露 selected unit cell，
        后续可以加 GetSelectedUnitCell() 来同步真实选中格。
    */
}

void AutoChessGame::BuildScene() {
    m_Scene.Clear();

    m_BoardSystem.SubmitRenderItems(
        m_Scene,
        m_BoardVisualLayer
    );

    m_UnitSystem.SubmitRenderItems(
        m_Scene
    );

    m_EffectVisualLayer.SubmitRenderItems(
        m_Scene
    );

    m_DamageNumberLayer.SubmitRenderItems(
        m_Scene,
        m_Camera,
        m_PixelConfig,
        m_LastFramebufferWidth,
        m_LastFramebufferHeight
    );
}

Engine::Camera& AutoChessGame::GetCamera() {
    return m_Camera;
}

const Engine::Scene& AutoChessGame::GetScene() const {
    return m_Scene;
}