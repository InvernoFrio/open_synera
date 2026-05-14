#include "Engine/Renderer/Vulkan/VulkanMesh.h"

namespace Engine {

    void VulkanMesh::Init(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        const Mesh& mesh
    ) {
        const auto& vertices = mesh.GetVertices();
        const auto& indices = mesh.GetIndices();

        m_IndexCount = static_cast<uint32_t>(indices.size());

        VkDeviceSize vertexBufferSize =
            sizeof(vertices[0]) * vertices.size();

        VkDeviceSize indexBufferSize =
            sizeof(indices[0]) * indices.size();

        m_VertexBuffer.Init(
            physicalDevice,
            device,
            vertexBufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        m_VertexBuffer.UploadData(
            vertices.data(),
            vertexBufferSize
        );

        m_IndexBuffer.Init(
            physicalDevice,
            device,
            indexBufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        m_IndexBuffer.UploadData(
            indices.data(),
            indexBufferSize
        );
    }

    void VulkanMesh::Shutdown() {
        m_IndexBuffer.Shutdown();
        m_VertexBuffer.Shutdown();

        m_IndexCount = 0;
    }

    void VulkanMesh::Bind(VkCommandBuffer commandBuffer) const {
        VkBuffer vertexBuffers[] = {
            m_VertexBuffer.GetBuffer()
        };

        VkDeviceSize offsets[] = {
            0
        };

        vkCmdBindVertexBuffers(
            commandBuffer,
            0,
            1,
            vertexBuffers,
            offsets
        );

        vkCmdBindIndexBuffer(
            commandBuffer,
            m_IndexBuffer.GetBuffer(),
            0,
            VK_INDEX_TYPE_UINT32
        );
    }

    void VulkanMesh::Draw(VkCommandBuffer commandBuffer) const {
        vkCmdDrawIndexed(
            commandBuffer,
            m_IndexCount,
            1,
            0,
            0,
            0
        );
    }

}