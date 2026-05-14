#pragma once

#include "Engine/Renderer/Material.h"

#include <glm/glm.hpp>

#include <optional>
#include <unordered_map>
#include <vector>

enum class BoardHighlightType {
    None = 0,

    Hover,
    Selected,
    MoveTarget,
    AttackRange,
    SkillRange,
    Invalid
};

struct BoardCellKey {
    int row = 0;
    int col = 0;

    bool operator==(const BoardCellKey& other) const {
        return row == other.row && col == other.col;
    }
};

struct BoardCellKeyHash {
    size_t operator()(const BoardCellKey& key) const {
        uint32_t r =
            static_cast<uint32_t>(key.row);

        uint32_t c =
            static_cast<uint32_t>(key.col);

        return
            (static_cast<size_t>(r) << 32) ^
            static_cast<size_t>(c);
    }
};

class BoardVisualLayer {
public:
    void Clear();

    void SetHoverCell(
        std::optional<glm::ivec2> cell
    );

    void SetSelectedCell(
        std::optional<glm::ivec2> cell
    );

    void AddHighlight(
        glm::ivec2 cell,
        BoardHighlightType type
    );

    void AddHighlights(
        const std::vector<glm::ivec2>& cells,
        BoardHighlightType type
    );

    BoardHighlightType GetHighlightType(
        int row,
        int col
    ) const;

    Engine::MaterialId GetMaterialForCell(
        int row,
        int col,
        bool defaultDark
    ) const;

private:
    Engine::MaterialId ToMaterial(
        BoardHighlightType type,
        bool defaultDark
    ) const;

private:
    std::optional<glm::ivec2> m_HoverCell;
    std::optional<glm::ivec2> m_SelectedCell;

    std::unordered_map<
        BoardCellKey,
        BoardHighlightType,
        BoardCellKeyHash
    > m_Highlights;
};