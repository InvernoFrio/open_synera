#pragma once

#include "Engine/Platform/Window.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Scene/Scene.h"

#include "Engine/Renderer/Vulkan/VulkanContext.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"
#include "Engine/Renderer/Vulkan/VulkanSwapchain.h"
#include "Engine/Renderer/Vulkan/VulkanRenderPass.h"
#include "Engine/Renderer/Vulkan/VulkanFramebuffer.h"
#include "Engine/Renderer/Vulkan/VulkanPipeline.h"
#include "Engine/Renderer/Vulkan/VulkanBuffer.h"
#include "Engine/Renderer/Vulkan/VulkanMesh.h"
#include "Engine/Renderer/Vulkan/VulkanDescriptor.h"
#include "Engine/Renderer/Vulkan/VulkanImage.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>

namespace Engine {

    struct CameraUBO {
        glm::mat4 viewProjection{ 1.0f };
    };

    struct MeshPushConstants {
        glm::mat4 model{ 1.0f };
        glm::vec4 color{ 1.0f };
    };

    class VulkanRenderer {
    public:
        void Init(Window& window);
        void Shutdown();

        bool DrawFrame(
            Window& window,
            const Camera& camera,
            const Scene& scene
        );

        VkExtent2D GetSwapchainExtent() const;

    private:
        static constexpr int MaxFramesInFlight = 2;

    private:
        void CreateSwapchainResources(Window& window);
        void CleanupSwapchainResources();

        void RecreateSwapchain(Window& window);

        void CreateCommandPool();
        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void CreateSyncObjects();
        void DestroySyncObjects();

        void CreateCameraBuffer();
        void CreateMeshes();
        void CreateDepthResources();

        void UpdateCameraBuffer(const Camera& camera);

        void RecordCommandBuffer(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex,
            const Scene& scene
        );

    private:
        VulkanContext m_Context;
        VulkanDevice m_Device;

        VulkanSwapchain m_Swapchain;
        VulkanImage m_DepthImage;

        VulkanRenderPass m_RenderPass;
        VulkanFramebuffer m_Framebuffer;

        VulkanBuffer m_CameraBuffer;
        VulkanDescriptor m_Descriptor;

        VulkanPipeline m_MeshPipeline;

        VulkanMesh m_BoardTileMesh;
        VulkanMesh m_UnitCubeMesh;

        MaterialLibrary m_MaterialLibrary;

        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_CommandBuffers;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;

        uint32_t m_CurrentFrame = 0;

        bool m_IsInitialized = false;
    };

}