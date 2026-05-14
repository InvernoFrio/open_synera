#pragma once

#include "Engine/Math/Ray.h"
#include "Engine/Scene/Scene.h"

#include <glm/glm.hpp>

#include <optional>

class BoardSystem {
public:
    void CreateBoard(
        int rows,
        int cols,
        float cellSize
    );

    void Update();

    void SubmitRenderItems(
        Engine::Scene& scene
    ) const;

    void HighlightCell(
        int row,
        int col
    );

    void ClearHighlight();

    std::optional<glm::ivec2> RaycastCell(
        const Engine::Ray& ray
    ) const;

    glm::vec3 CellToWorld(
        int row,
        int col
    ) const;

    bool IsValidCell(
        int row,
        int col
    ) const;

    int GetRows() const;
    int GetCols() const;
    float GetCellSize() const;

private:
    int m_Rows = 0;
    int m_Cols = 0;
    float m_CellSize = 1.0f;

    glm::ivec2 m_HighlightedCell{ -1, -1 };
};