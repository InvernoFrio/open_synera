#pragma once

#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Vulkan/VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanMesh {
    public:
        void Init(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            const Mesh& mesh
        );

        void Shutdown();

        void Bind(VkCommandBuffer commandBuffer) const;
        void Draw(VkCommandBuffer commandBuffer) const;

    private:
        VulkanBuffer m_VertexBuffer;
        VulkanBuffer m_IndexBuffer;

        uint32_t m_IndexCount = 0;
    };

}