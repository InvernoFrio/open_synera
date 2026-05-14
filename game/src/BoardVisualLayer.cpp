#include "BoardVisualLayer.h"

void BoardVisualLayer::Clear() {
    m_HoverCell.reset();
    m_SelectedCell.reset();
    m_Highlights.clear();
}

void BoardVisualLayer::SetHoverCell(
    std::optional<glm::ivec2> cell
) {
    m_HoverCell = cell;
}

void BoardVisualLayer::SetSelectedCell(
    std::optional<glm::ivec2> cell
) {
    m_SelectedCell = cell;
}

void BoardVisualLayer::AddHighlight(
    glm::ivec2 cell,
    BoardHighlightType type
) {
    BoardCellKey key;
    key.row = cell.x;
    key.col = cell.y;

    m_Highlights[key] = type;
}

void BoardVisualLayer::AddHighlights(
    const std::vector<glm::ivec2>& cells,
    BoardHighlightType type
) {
    for (glm::ivec2 cell : cells) {
        AddHighlight(
            cell,
            type
        );
    }
}

BoardHighlightType BoardVisualLayer::GetHighlightType(
    int row,
    int col
) const {
    /*
        优先级：
        Invalid > Hover > Selected > SkillRange > AttackRange > MoveTarget > None

        你可以根据游戏表现调整。
    */

    BoardCellKey key;
    key.row = row;
    key.col = col;

    auto it =
        m_Highlights.find(key);

    if (it != m_Highlights.end()) {
        if (it->second == BoardHighlightType::Invalid) {
            return BoardHighlightType::Invalid;
        }
    }

    if (m_HoverCell.has_value() &&
        m_HoverCell->x == row &&
        m_HoverCell->y == col) {
        return BoardHighlightType::Hover;
    }

    if (m_SelectedCell.has_value() &&
        m_SelectedCell->x == row &&
        m_SelectedCell->y == col) {
        return BoardHighlightType::Selected;
    }

    if (it != m_Highlights.end()) {
        return it->second;
    }

    return BoardHighlightType::None;
}

Engine::MaterialId BoardVisualLayer::GetMaterialForCell(
    int row,
    int col,
    bool defaultDark
) const {
    BoardHighlightType type =
        GetHighlightType(
            row,
            col
        );

    return ToMaterial(
        type,
        defaultDark
    );
}

Engine::MaterialId BoardVisualLayer::ToMaterial(
    BoardHighlightType type,
    bool defaultDark
) const {
    switch (type) {
    case BoardHighlightType::Hover:
        return Engine::MaterialId::BoardHover;

    case BoardHighlightType::Selected:
        return Engine::MaterialId::BoardSelected;

    case BoardHighlightType::MoveTarget:
        return Engine::MaterialId::BoardMoveTarget;

    case BoardHighlightType::AttackRange:
        return Engine::MaterialId::BoardAttackRange;

    case BoardHighlightType::SkillRange:
        return Engine::MaterialId::BoardSkillRange;

    case BoardHighlightType::Invalid:
        return Engine::MaterialId::BoardInvalid;

    case BoardHighlightType::None:
    default:
        return defaultDark
            ? Engine::MaterialId::BoardDark
            : Engine::MaterialId::BoardLight;
    }
}