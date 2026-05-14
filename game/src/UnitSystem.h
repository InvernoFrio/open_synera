#pragma once

#include "Engine/Renderer/Material.h"
#include "Engine/Scene/Scene.h"

#include <glm/glm.hpp>

#include <optional>
#include <vector>
#include <cstdint>

struct Unit {
    uint32_t id = 0;

    glm::ivec2 cell{ 0, 0 };

    glm::vec3 position{ 0.0f };
    glm::vec3 targetPosition{ 0.0f };

    Engine::MaterialId materialId =
        Engine::MaterialId::DebugWhite;

    float height = 1.0f;
    bool selected = false;
};

class UnitSystem {
public:
    void InitBoardInfo(
        int rows,
        int cols,
        float cellSize
    );

    uint32_t SpawnUnit(
        glm::ivec2 cell,
        Engine::MaterialId materialId,
        float height
    );

    void Update(float dt);

    void SubmitRenderItems(
        Engine::Scene& scene
    ) const;

    std::optional<uint32_t> FindUnitAtCell(
        glm::ivec2 cell
    ) const;

    void SelectUnit(uint32_t id);
    void ClearSelection();

    bool HasSelection() const;
    uint32_t GetSelectedUnit() const;

    void MoveSelectedUnitToCell(
        glm::ivec2 cell
    );

private:
    glm::vec3 CellToWorld(
        glm::ivec2 cell
    ) const;

    Unit* FindUnit(uint32_t id);
    const Unit* FindUnit(uint32_t id) const;

private:
    uint32_t m_NextId = 1;

    int m_BoardRows = 8;
    int m_BoardCols = 8;
    float m_CellSize = 1.2f;

    uint32_t m_SelectedUnit = 0;

    std::vector<Unit> m_Units;
};