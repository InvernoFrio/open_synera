#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>

namespace Engine {

    struct MeshVertex {
        glm::vec3 position;
        glm::vec3 normal;

        static VkVertexInputBindingDescription GetBindingDescription() {
            VkVertexInputBindingDescription binding{};
            binding.binding = 0;
            binding.stride = sizeof(MeshVertex);
            binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding;
        }

        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributes{};

            attributes[0].binding = 0;
            attributes[0].location = 0;
            attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributes[0].offset = offsetof(MeshVertex, position);

            attributes[1].binding = 0;
            attributes[1].location = 1;
            attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributes[1].offset = offsetof(MeshVertex, normal);

            return attributes;
        }
    };

    struct MeshData {
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
    };

    class Mesh {
    public:
        void SetData(const MeshData& data);

        const std::vector<MeshVertex>& GetVertices() const;
        const std::vector<uint32_t>& GetIndices() const;

        static Mesh CreateQuad();
        static Mesh CreateCube();

    private:
        MeshData m_Data;
    };

}