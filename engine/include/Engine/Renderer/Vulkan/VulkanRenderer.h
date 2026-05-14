#pragma once

#include "Engine/Platform/Window.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Renderer/PixelRenderConfig.h"
#include "Engine/Renderer/OverlayRenderItem.h"
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
#include "Engine/Renderer/Vulkan/VulkanOffscreenRenderTarget.h"
#include "Engine/Renderer/Vulkan/VulkanFullscreenPass.h"
#include "Engine/Renderer/Vulkan/VulkanSpritePipeline.h"
#include "Engine/Renderer/Vulkan/VulkanOverlayPipeline.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>

namespace Engine {

    struct CameraUBO {
        glm::mat4 viewProjection{ 1.0f };

        glm::vec4 cameraRight{
            1.0f,
            0.0f,
            0.0f,
            0.0f
        };

        glm::vec4 cameraUp{
            0.0f,
            1.0f,
            0.0f,
            0.0f
        };
    };

    struct SpritePushConstants {
        glm::vec4 positionSize{ 0.0f };
        glm::vec4 color{ 1.0f };
        glm::vec4 params{ 1.0f };
    };

    struct MeshPushConstants {
        glm::mat4 model{ 1.0f };
        glm::vec4 color{ 1.0f };

        glm::vec4 shadingParams{
        3.0f,   // x = shadeSteps
        0.55f,  // y = shadowStrength
        0.35f,  // z = ambient
        0.0f    // w = unused
        };

        glm::vec4 outlineParams{
        0.0f,   // x = renderMode, 0 normal, 1 outline
        0.0f,   // y = outlineWidth
        0.0f,
        0.0f
        };
    };

    struct OverlayPushConstants {
        glm::vec4 rect{ 0.0f };
        glm::vec4 color{ 1.0f };
        glm::vec4 params{ 0.0f };
        glm::vec4 extra{ 0.0f };
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

        const PixelRenderConfig& GetPixelRenderConfig() const;

    private:
        static constexpr int MaxFramesInFlight = 2;

    private:
        void CreateSwapchainResources(Window& window);
        void CleanupSwapchainResources();

        void CreatePixelResources();
        void CleanupPixelResources();

        void RecreateSwapchain(Window& window);

        void CreateCommandPool();
        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void CreateSyncObjects();
        void DestroySyncObjects();

        void CreateCameraBuffer();
        void CreateMeshes();

        void UpdateCameraBuffer(const Camera& camera);

        void RecordCommandBuffer(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex,
            const Scene& scene
        );

        void RecordScenePass(
            VkCommandBuffer commandBuffer,
            const Scene& scene
        );

        void RecordPresentPass(
            VkCommandBuffer commandBuffer,
            uint32_t imageIndex
        );

    private:
        PixelRenderConfig m_PixelConfig;

        VulkanContext m_Context;
        VulkanDevice m_Device;

        VulkanSwapchain m_Swapchain;

        VulkanOffscreenRenderTarget m_OffscreenTarget;

        VulkanRenderPass m_PresentRenderPass;
        VulkanFramebuffer m_PresentFramebuffer;

        VulkanBuffer m_CameraBuffer;
        VulkanDescriptor m_Descriptor;

        VulkanPipeline m_MeshPipeline;
        VulkanPipeline m_OutlinePipeline;
        VulkanSpritePipeline m_SpritePipeline;
        VulkanOverlayPipeline m_OverlayPipeline;

        VulkanFullscreenPass m_FullscreenPass;

        VulkanMesh m_BoardTileMesh;
        VulkanMesh m_UnitCubeMesh;

        MaterialLibrary m_MaterialLibrary;

        VkCommandPool m_CommandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_CommandBuffers;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;

        std::vector<OverlayRenderItem> m_CurrentSceneOverlays;

        uint32_t m_CurrentFrame = 0;

        bool m_IsInitialized = false;
    };

}