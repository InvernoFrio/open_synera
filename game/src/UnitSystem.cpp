#include "UnitSystem.h"

#include "Engine/Renderer/RenderItem.h"

#include <glm/gtc/matrix_transform.hpp>

void UnitSystem::InitBoardInfo(
    int rows,
    int cols,
    float cellSize
) {
    m_BoardRows = rows;
    m_BoardCols = cols;
    m_CellSize = cellSize;
}

uint32_t UnitSystem::SpawnUnit(
    glm::ivec2 cell,
    Engine::MaterialId materialId,
    float height
) {
    Unit unit;
    unit.id = m_NextId++;
    unit.cell = cell;
    unit.position = CellToWorld(cell);
    unit.targetPosition = unit.position;
    unit.materialId = materialId;
    unit.height = height;

    m_Units.push_back(unit);

    return unit.id;
}

void UnitSystem::Update(float dt) {
    for (Unit& unit : m_Units) {
        float moveSpeed = 10.0f;

        float alpha =
            glm::clamp(
                dt * moveSpeed,
                0.0f,
                1.0f
            );

        unit.position =
            glm::mix(
                unit.position,
                unit.targetPosition,
                alpha
            );
    }
}

void UnitSystem::SubmitRenderItems(
    Engine::Scene& scene
) const {
    for (const Unit& unit : m_Units) {
        glm::vec3 renderPosition =
            unit.position;

        renderPosition.y =
            unit.height * 0.5f;

        glm::mat4 model{ 1.0f };

        model = glm::translate(
            model,
            renderPosition
        );

        model = glm::scale(
            model,
            glm::vec3{
                unit.selected ? 0.70f : 0.55f,
                unit.height,
                unit.selected ? 0.70f : 0.55f
            }
        );

        Engine::RenderItem item;
        item.meshType = Engine::MeshType::Cube;
        item.materialId = unit.selected
            ? Engine::MaterialId::UnitSelected
            : unit.materialId;

        item.model = model;

        item.flags =
            static_cast<uint32_t>(
                Engine::RenderFlags::Outline
                );

        item.outlineMaterialId =
            Engine::MaterialId::OutlineDark;

        item.outlineWidth =
            unit.selected ? 0.055f : 0.035f;

        scene.Submit(item);
    }
}

std::optional<uint32_t> UnitSystem::FindUnitAtCell(
    glm::ivec2 cell
) const {
    for (const Unit& unit : m_Units) {
        if (unit.cell == cell) {
            return unit.id;
        }
    }

    return std::nullopt;
}

void UnitSystem::SelectUnit(uint32_t id) {
    ClearSelection();

    Unit* unit =
        FindUnit(id);

    if (!unit) {
        return;
    }

    unit->selected = true;
    m_SelectedUnit = id;
}

void UnitSystem::ClearSelection() {
    for (Unit& unit : m_Units) {
        unit.selected = false;
    }

    m_SelectedUnit = 0;
}

bool UnitSystem::HasSelection() const {
    return m_SelectedUnit != 0;
}

uint32_t UnitSystem::GetSelectedUnit() const {
    return m_SelectedUnit;
}

std::optional<glm::ivec2> UnitSystem::GetSelectedUnitCell() const {
    if (!HasSelection()) {
        return std::nullopt;
    }

    const Unit* unit =
        FindUnit(m_SelectedUnit);

    if (!unit) {
        return std::nullopt;
    }

    return unit->cell;
}

void UnitSystem::MoveSelectedUnitToCell(
    glm::ivec2 cell
) {
    if (!HasSelection()) {
        return;
    }

    Unit* unit =
        FindUnit(m_SelectedUnit);

    if (!unit) {
        return;
    }

    unit->cell = cell;
    unit->targetPosition =
        CellToWorld(cell);
}

glm::vec3 UnitSystem::CellToWorld(
    glm::ivec2 cell
) const {
    float x =
        (static_cast<float>(cell.y) - static_cast<float>(m_BoardCols) * 0.5f + 0.5f)
        * m_CellSize;

    float z =
        (static_cast<float>(cell.x) - static_cast<float>(m_BoardRows) * 0.5f + 0.5f)
        * m_CellSize;

    return glm::vec3{
        x,
        0.0f,
        z
    };
}

Unit* UnitSystem::FindUnit(
    uint32_t id
) {
    for (Unit& unit : m_Units) {
        if (unit.id == id) {
            return &unit;
        }
    }

    return nullptr;
}

const Unit* UnitSystem::FindUnit(
    uint32_t id
) const {
    for (const Unit& unit : m_Units) {
        if (unit.id == id) {
            return &unit;
        }
    }

    return nullptr;
}