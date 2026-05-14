#include "Engine/Renderer/Mesh.h"

namespace Engine {

    void Mesh::SetData(const MeshData& data) {
        m_Data = data;
    }

    const std::vector<MeshVertex>& Mesh::GetVertices() const {
        return m_Data.vertices;
    }

    const std::vector<uint32_t>& Mesh::GetIndices() const {
        return m_Data.indices;
    }

    Mesh Mesh::CreateQuad() {
        Mesh mesh;

        MeshData data;

        // 棋盘平面在 XZ 平面，朝上，所以 normal 是 +Y
        data.vertices = {
            { {-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f} },
            { { 0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f} },
            { { 0.5f, 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f} },
            { {-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f} }
        };

        data.indices = {
            0, 1, 2,
            2, 3, 0
        };

        mesh.SetData(data);
        return mesh;
    }

    Mesh Mesh::CreateCube() {
        Mesh mesh;

        MeshData data;

        /*
            为了让 Cube 每个面有正确的平面法线，
            不能只用 8 个共享顶点。

            如果 8 个顶点共享，会导致法线在棱角处被平滑，
            Cube 看起来像圆润的低模体。

            所以这里每个面 4 个顶点，一共 24 个顶点。
        */

        data.vertices = {
            // Front face, +Z
            { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
            { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
            { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },
            { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} },

            // Back face, -Z
            { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },
            { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },
            { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },
            { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f} },

            // Left face, -X
            { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f} },
            { {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f} },
            { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f} },
            { {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f} },

            // Right face, +X
            { {0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f} },
            { {0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} },
            { {0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} },
            { {0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f} },

            // Top face, +Y
            { {-0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f} },
            { { 0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f} },
            { { 0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },
            { {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} },

            // Bottom face, -Y
            { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f} },
            { { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f} },
            { { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f} },
            { {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f} }
        };

        data.indices = {
            // Front
            0, 1, 2,
            2, 3, 0,

            // Back
            4, 5, 6,
            6, 7, 4,

            // Left
            8, 9, 10,
            10, 11, 8,

            // Right
            12, 13, 14,
            14, 15, 12,

            // Top
            16, 17, 18,
            18, 19, 16,

            // Bottom
            20, 21, 22,
            22, 23, 20
        };

        mesh.SetData(data);
        return mesh;
    }

}