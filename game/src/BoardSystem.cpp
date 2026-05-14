#include "BoardSystem.h"

#include "Engine/Renderer/RenderItem.h"

#include <glm/gtc/matrix_transform.hpp>

void BoardSystem::CreateBoard(
    int rows,
    int cols,
    float cellSize
) {
    m_Rows = rows;
    m_Cols = cols;
    m_CellSize = cellSize;
}

void BoardSystem::Update() {
}

void BoardSystem::SubmitRenderItems(
    Engine::Scene& scene,
    const BoardVisualLayer& visualLayer
) const {
    for (int row = 0; row < m_Rows; row++) {
        for (int col = 0; col < m_Cols; col++) {
            glm::vec3 position =
                CellToWorld(
                    row,
                    col
                );

            glm::mat4 model{ 1.0f };

            model = glm::translate(
                model,
                position
            );

            model = glm::scale(
                model,
                glm::vec3{
                    m_CellSize * 0.95f,
                    1.0f,
                    m_CellSize * 0.95f
                }
            );

            bool dark =
                ((row + col) % 2) == 0;

            Engine::MaterialId materialId =
                visualLayer.GetMaterialForCell(
                    row,
                    col,
                    dark
                );

            Engine::RenderItem item;
            item.meshType = Engine::MeshType::Quad;
            item.materialId = materialId;
            item.model = model;

            scene.Submit(item);
        }
    }
}

std::optional<glm::ivec2> BoardSystem::RaycastCell(
    const Engine::Ray& ray
) const {
    if (glm::abs(ray.direction.y) < 0.00001f) {
        return std::nullopt;
    }

    float t =
        -ray.origin.y / ray.direction.y;

    if (t < 0.0f) {
        return std::nullopt;
    }

    glm::vec3 hitPoint =
        ray.origin + ray.direction * t;

    float boardWidth =
        static_cast<float>(m_Cols) * m_CellSize;

    float boardHeight =
        static_cast<float>(m_Rows) * m_CellSize;

    float localX =
        hitPoint.x + boardWidth * 0.5f;

    float localZ =
        hitPoint.z + boardHeight * 0.5f;

    if (localX < 0.0f ||
        localZ < 0.0f ||
        localX >= boardWidth ||
        localZ >= boardHeight) {
        return std::nullopt;
    }

    int col =
        static_cast<int>(localX / m_CellSize);

    int row =
        static_cast<int>(localZ / m_CellSize);

    if (!IsValidCell(row, col)) {
        return std::nullopt;
    }

    return glm::ivec2{
        row,
        col
    };
}

glm::vec3 BoardSystem::CellToWorld(
    int row,
    int col
) const {
    float x =
        (static_cast<float>(col) - static_cast<float>(m_Cols) * 0.5f + 0.5f)
        * m_CellSize;

    float z =
        (static_cast<float>(row) - static_cast<float>(m_Rows) * 0.5f + 0.5f)
        * m_CellSize;

    return glm::vec3{
        x,
        0.0f,
        z
    };
}

bool BoardSystem::IsValidCell(
    int row,
    int col
) const {
    return
        row >= 0 &&
        row < m_Rows &&
        col >= 0 &&
        col < m_Cols;
}

int BoardSystem::GetRows() const {
    return m_Rows;
}

int BoardSystem::GetCols() const {
    return m_Cols;
}

float BoardSystem::GetCellSize() const {
    return m_CellSize;
}