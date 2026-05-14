#include "Engine/Renderer/Vulkan/VulkanRenderer.h"

#include <array>
#include <stdexcept>

namespace Engine {

    void VulkanRenderer::Init(Window& window) {
#ifdef ENGINE_DEBUG
        constexpr bool enableValidation = true;
#else
        constexpr bool enableValidation = false;
#endif

        m_Context.Init(
            window.GetNativeHandle(),
            enableValidation
        );

        m_Device.Init(
            m_Context.GetInstance(),
            m_Context.GetSurface()
        );

        CreateCommandPool();

        CreateCameraBuffer();

        m_Descriptor.Init(
            m_Device.GetDevice(),
            m_CameraBuffer.GetBuffer(),
            m_CameraBuffer.GetSize()
        );

        CreateMeshes();

        m_MaterialLibrary.InitDefaultMaterials();

        CreatePixelResources();
        CreateSwapchainResources(window);

        CreateCommandBuffers();
        CreateSyncObjects();

        m_IsInitialized = true;
    }

    void VulkanRenderer::Shutdown() {
        VkDevice device =
            m_Device.GetDevice();

        if (device != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(device);
        }

        DestroySyncObjects();
        FreeCommandBuffers();

        CleanupSwapchainResources();
        CleanupPixelResources();

        if (device != VK_NULL_HANDLE) {
            m_UnitCubeMesh.Shutdown();
            m_BoardTileMesh.Shutdown();

            m_Descriptor.Shutdown();
            m_CameraBuffer.Shutdown();

            if (m_CommandPool != VK_NULL_HANDLE) {
                vkDestroyCommandPool(
                    device,
                    m_CommandPool,
                    nullptr
                );

                m_CommandPool = VK_NULL_HANDLE;
            }
        }

        m_Device.Shutdown();
        m_Context.Shutdown();

        m_IsInitialized = false;
    }

    bool VulkanRenderer::DrawFrame(
        Window& window,
        const Camera& camera,
        const Scene& scene
    ) {
        if (!m_IsInitialized) {
            return false;
        }

        if (window.GetFramebufferWidth() == 0 ||
            window.GetFramebufferHeight() == 0) {
            return false;
        }

        UpdateCameraBuffer(camera);

        VkDevice device =
            m_Device.GetDevice();

        vkWaitForFences(
            device,
            1,
            &m_InFlightFences[m_CurrentFrame],
            VK_TRUE,
            UINT64_MAX
        );

        uint32_t imageIndex = 0;

        VkResult acquireResult =
            vkAcquireNextImageKHR(
                device,
                m_Swapchain.GetSwapchain(),
                UINT64_MAX,
                m_ImageAvailableSemaphores[m_CurrentFrame],
                VK_NULL_HANDLE,
                &imageIndex
            );

        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
            RecreateSwapchain(window);
            return false;
        }

        if (acquireResult != VK_SUCCESS &&
            acquireResult != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swapchain image");
        }

        vkResetFences(
            device,
            1,
            &m_InFlightFences[m_CurrentFrame]
        );

        vkResetCommandBuffer(
            m_CommandBuffers[imageIndex],
            0
        );

        RecordCommandBuffer(
            m_CommandBuffers[imageIndex],
            imageIndex,
            scene
        );

        VkSemaphore waitSemaphores[] = {
            m_ImageAvailableSemaphores[m_CurrentFrame]
        };

        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };

        VkSemaphore signalSemaphores[] = {
            m_RenderFinishedSemaphores[m_CurrentFrame]
        };

        VkSubmitInfo submitInfo{};
        submitInfo.sType =
            VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers =
            &m_CommandBuffers[imageIndex];

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores =
            signalSemaphores;

        if (vkQueueSubmit(
            m_Device.GetGraphicsQueue(),
            1,
            &submitInfo,
            m_InFlightFences[m_CurrentFrame]
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit command buffer");
        }

        VkSwapchainKHR swapchains[] = {
            m_Swapchain.GetSwapchain()
        };

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType =
            VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores =
            signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        VkResult presentResult =
            vkQueuePresentKHR(
                m_Device.GetPresentQueue(),
                &presentInfo
            );

        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR ||
            presentResult == VK_SUBOPTIMAL_KHR ||
            window.WasResized()) {
            window.ResetResizeFlag();
            RecreateSwapchain(window);
        }
        else if (presentResult != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swapchain image");
        }

        m_CurrentFrame =
            (m_CurrentFrame + 1) % MaxFramesInFlight;

        return true;
    }

    VkExtent2D VulkanRenderer::GetSwapchainExtent() const {
        return m_Swapchain.GetExtent();
    }

    const PixelRenderConfig& VulkanRenderer::GetPixelRenderConfig() const {
        return m_PixelConfig;
    }

    void VulkanRenderer::CreatePixelResources() {
        m_OffscreenTarget.Init(
            m_Device.GetPhysicalDevice(),
            m_Device.GetDevice(),
            m_PixelConfig.internalWidth,
            m_PixelConfig.internalHeight
        );

        VulkanPipelineConfig meshConfig{};
        meshConfig.cullMode = VK_CULL_MODE_NONE;
        meshConfig.depthTestEnable = true;
        meshConfig.depthWriteEnable = true;
        meshConfig.depthCompareOp = VK_COMPARE_OP_LESS;

        m_MeshPipeline.Init(
            m_Device.GetDevice(),
            m_OffscreenTarget.GetExtent(),
            m_OffscreenTarget.GetRenderPass(),
            m_Descriptor.GetLayout(),
            "assets/shaders/mesh.vert.spv",
            "assets/shaders/mesh.frag.spv",
            meshConfig
        );

        VulkanPipelineConfig outlineConfig{};
        outlineConfig.cullMode = VK_CULL_MODE_NONE;

        /*
            简化方案：
            描边 pass 不读写 depth。
            先画黑色膨胀模型，再画正常模型。
            正常模型会覆盖内部，只保留边缘外扩部分。

            优点：
                稳定，简单，不容易因为 winding order 出错。

            缺点：
                部分重叠情况下，远处物体的描边可能会透出。
                后续可以升级为 inverted hull + depth test。
        */
        outlineConfig.depthTestEnable = false;
        outlineConfig.depthWriteEnable = false;
        outlineConfig.depthCompareOp = VK_COMPARE_OP_ALWAYS;

        m_OutlinePipeline.Init(
            m_Device.GetDevice(),
            m_OffscreenTarget.GetExtent(),
            m_OffscreenTarget.GetRenderPass(),
            m_Descriptor.GetLayout(),
            "assets/shaders/mesh.vert.spv",
            "assets/shaders/mesh.frag.spv",
            outlineConfig
        );

        m_SpritePipeline.Init(
            m_Device.GetDevice(),
            m_OffscreenTarget.GetExtent(),
            m_OffscreenTarget.GetRenderPass(),
            m_Descriptor.GetLayout()
        );
    }

    void VulkanRenderer::CleanupPixelResources() {
        m_SpritePipeline.Shutdown();

        m_MeshPipeline.Shutdown();
        m_OutlinePipeline.Shutdown();

        m_OffscreenTarget.Shutdown();
    }

    void VulkanRenderer::CreateSwapchainResources(Window& window) {
        QueueFamilyIndices queueFamilyIndices =
            m_Device.FindQueueFamilies(
                m_Device.GetPhysicalDevice()
            );

        m_Swapchain.Init(
            m_Device.GetPhysicalDevice(),
            m_Device.GetDevice(),
            m_Context.GetSurface(),
            window.GetFramebufferWidth(),
            window.GetFramebufferHeight(),
            queueFamilyIndices
        );

        m_PresentRenderPass.InitPresent(
            m_Device.GetDevice(),
            m_Swapchain.GetImageFormat()
        );

        m_PresentFramebuffer.InitSwapchainColorOnly(
            m_Device.GetDevice(),
            m_PresentRenderPass.Get(),
            m_Swapchain.GetImageViews(),
            m_Swapchain.GetExtent()
        );

        m_FullscreenPass.Init(
            m_Device.GetDevice(),
            m_Swapchain.GetExtent(),
            m_PresentRenderPass.Get(),
            m_OffscreenTarget.GetColorImageView(),
            m_OffscreenTarget.GetColorSampler()
        );
    }

    void VulkanRenderer::CleanupSwapchainResources() {
        VkDevice device =
            m_Device.GetDevice();

        if (device != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(device);
        }

        m_FullscreenPass.Shutdown();

        m_PresentFramebuffer.Shutdown();
        m_PresentRenderPass.Shutdown();

        m_Swapchain.Shutdown();
    }

    void VulkanRenderer::RecreateSwapchain(Window& window) {
        while (window.GetFramebufferWidth() == 0 ||
            window.GetFramebufferHeight() == 0) {
            window.PollEvents();
        }

        vkDeviceWaitIdle(
            m_Device.GetDevice()
        );

        FreeCommandBuffers();

        CleanupSwapchainResources();

        CreateSwapchainResources(window);

        CreateCommandBuffers();
    }

    void VulkanRenderer::CreateCameraBuffer() {
        m_CameraBuffer.Init(
            m_Device.GetPhysicalDevice(),
            m_Device.GetDevice(),
            sizeof(CameraUBO),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
    }

    void VulkanRenderer::CreateMeshes() {
        Mesh quad =
            Mesh::CreateQuad();

        m_BoardTileMesh.Init(
            m_Device.GetPhysicalDevice(),
            m_Device.GetDevice(),
            quad
        );

        Mesh cube =
            Mesh::CreateCube();

        m_UnitCubeMesh.Init(
            m_Device.GetPhysicalDevice(),
            m_Device.GetDevice(),
            cube
        );
    }

    void VulkanRenderer::CreateCommandPool() {
        QueueFamilyIndices queueFamilyIndices =
            m_Device.FindQueueFamilies(
                m_Device.GetPhysicalDevice()
            );

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex =
            queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(
            m_Device.GetDevice(),
            &poolInfo,
            nullptr,
            &m_CommandPool
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool");
        }
    }

    void VulkanRenderer::CreateCommandBuffers() {
        m_CommandBuffers.resize(
            m_Swapchain.GetImageViews().size()
        );

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level =
            VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount =
            static_cast<uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(
            m_Device.GetDevice(),
            &allocInfo,
            m_CommandBuffers.data()
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers");
        }
    }

    void VulkanRenderer::FreeCommandBuffers() {
        if (m_CommandBuffers.empty()) {
            return;
        }

        vkFreeCommandBuffers(
            m_Device.GetDevice(),
            m_CommandPool,
            static_cast<uint32_t>(m_CommandBuffers.size()),
            m_CommandBuffers.data()
        );

        m_CommandBuffers.clear();
    }

    void VulkanRenderer::CreateSyncObjects() {
        m_ImageAvailableSemaphores.resize(
            MaxFramesInFlight
        );

        m_RenderFinishedSemaphores.resize(
            MaxFramesInFlight
        );

        m_InFlightFences.resize(
            MaxFramesInFlight
        );

        VkDevice device =
            m_Device.GetDevice();

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType =
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType =
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags =
            VK_FENCE_CREATE_SIGNALED_BIT;

        for (int i = 0; i < MaxFramesInFlight; i++) {
            if (vkCreateSemaphore(
                device,
                &semaphoreInfo,
                nullptr,
                &m_ImageAvailableSemaphores[i]
            ) != VK_SUCCESS ||
                vkCreateSemaphore(
                    device,
                    &semaphoreInfo,
                    nullptr,
                    &m_RenderFinishedSemaphores[i]
                ) != VK_SUCCESS ||
                vkCreateFence(
                    device,
                    &fenceInfo,
                    nullptr,
                    &m_InFlightFences[i]
                ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create sync objects");
            }
        }
    }

    void VulkanRenderer::DestroySyncObjects() {
        VkDevice device =
            m_Device.GetDevice();

        if (device == VK_NULL_HANDLE) {
            return;
        }

        for (VkFence fence : m_InFlightFences) {
            if (fence != VK_NULL_HANDLE) {
                vkDestroyFence(
                    device,
                    fence,
                    nullptr
                );
            }
        }

        for (VkSemaphore semaphore : m_RenderFinishedSemaphores) {
            if (semaphore != VK_NULL_HANDLE) {
                vkDestroySemaphore(
                    device,
                    semaphore,
                    nullptr
                );
            }
        }

        for (VkSemaphore semaphore : m_ImageAvailableSemaphores) {
            if (semaphore != VK_NULL_HANDLE) {
                vkDestroySemaphore(
                    device,
                    semaphore,
                    nullptr
                );
            }
        }

        m_InFlightFences.clear();
        m_RenderFinishedSemaphores.clear();
        m_ImageAvailableSemaphores.clear();
    }

    void VulkanRenderer::UpdateCameraBuffer(
        const Camera& camera
    ) {
        CameraUBO ubo{};
        ubo.viewProjection =
            camera.GetViewProjection();

        glm::vec3 right =
            camera.GetRight();

        glm::vec3 up =
            camera.GetUp();

        ubo.cameraRight =
            glm::vec4{
                right,
                0.0f
        };

        ubo.cameraUp =
            glm::vec4{
                up,
                0.0f
        };

        m_CameraBuffer.UploadData(
            &ubo,
            sizeof(CameraUBO)
        );
    }

    void VulkanRenderer::RecordCommandBuffer(
        VkCommandBuffer commandBuffer,
        uint32_t imageIndex,
        const Scene& scene
    ) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(
            commandBuffer,
            &beginInfo
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin command buffer");
        }

        RecordScenePass(
            commandBuffer,
            scene
        );

        RecordPresentPass(
            commandBuffer,
            imageIndex
        );

        if (vkEndCommandBuffer(
            commandBuffer
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer");
        }
    }

    void VulkanRenderer::RecordScenePass(
        VkCommandBuffer commandBuffer,
        const Scene& scene
    ) {
        std::array<VkClearValue, 2> clearValues{};

        clearValues[0].color = {
            { 0.06f, 0.07f, 0.09f, 1.0f }
        };

        clearValues[1].depthStencil = {
            1.0f,
            0
        };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType =
            VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass =
            m_OffscreenTarget.GetRenderPass();
        renderPassInfo.framebuffer =
            m_OffscreenTarget.GetFramebuffer();
        renderPassInfo.renderArea.offset = {
            0,
            0
        };
        renderPassInfo.renderArea.extent =
            m_OffscreenTarget.GetExtent();
        renderPassInfo.clearValueCount =
            static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues =
            clearValues.data();

        vkCmdBeginRenderPass(
            commandBuffer,
            &renderPassInfo,
            VK_SUBPASS_CONTENTS_INLINE
        );

        VkDescriptorSet descriptorSet =
            m_Descriptor.GetSet();

        auto drawItem =
            [&](const RenderItem& item, bool outlineMode) {
            const VulkanMesh* mesh = nullptr;

            switch (item.meshType) {
            case MeshType::Quad:
                mesh = &m_BoardTileMesh;
                break;

            case MeshType::Cube:
                mesh = &m_UnitCubeMesh;
                break;

            default:
                break;
            }

            if (!mesh) {
                return;
            }

            MaterialId materialId =
                outlineMode
                ? item.outlineMaterialId
                : item.materialId;

            const Material& material =
                m_MaterialLibrary.GetMaterial(
                    materialId
                );

            mesh->Bind(commandBuffer);

            MeshPushConstants push{};
            push.model = item.model;
            push.color = material.baseColor;
            push.shadingParams = glm::vec4{
                material.shadeSteps,
                material.shadowStrength,
                material.ambient,
                0.0f
            };

            push.outlineParams = glm::vec4{
                outlineMode ? 1.0f : 0.0f,
                outlineMode ? item.outlineWidth : 0.0f,
                0.0f,
                0.0f
            };

            vkCmdPushConstants(
                commandBuffer,
                outlineMode
                ? m_OutlinePipeline.GetPipelineLayout()
                : m_MeshPipeline.GetPipelineLayout(),
                VK_SHADER_STAGE_VERTEX_BIT |
                VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(MeshPushConstants),
                &push
            );

            mesh->Draw(commandBuffer);
            };

        /*
            Pass 1：描边
        */
        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_OutlinePipeline.GetPipeline()
        );

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_OutlinePipeline.GetPipelineLayout(),
            0,
            1,
            &descriptorSet,
            0,
            nullptr
        );

        for (const RenderItem& item : scene.GetRenderItems()) {
            if (!HasRenderFlag(
                item.flags,
                RenderFlags::Outline
            )) {
                continue;
            }

            drawItem(
                item,
                true
            );
        }

        /*
            Pass 2：正常物体
        */
        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_MeshPipeline.GetPipeline()
        );

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_MeshPipeline.GetPipelineLayout(),
            0,
            1,
            &descriptorSet,
            0,
            nullptr
        );

        for (const RenderItem& item : scene.GetRenderItems()) {
            drawItem(
                item,
                false
            );
        }

        /*
            Pass 3：Sprite / Billboard 特效
        */
        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_SpritePipeline.GetPipeline()
        );

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_SpritePipeline.GetPipelineLayout(),
            0,
            1,
            &descriptorSet,
            0,
            nullptr
        );

        for (const SpriteRenderItem& sprite : scene.GetSpriteRenderItems()) {
            SpritePushConstants push{};

            push.positionSize =
                glm::vec4{
                    sprite.worldPosition,
                    0.0f
            };

            push.color =
                sprite.color;

            push.params =
                glm::vec4{
                    sprite.size.x,
                    sprite.size.y,
                    sprite.rotationRadians,
                    sprite.spriteType
            };

            vkCmdPushConstants(
                commandBuffer,
                m_SpritePipeline.GetPipelineLayout(),
                VK_SHADER_STAGE_VERTEX_BIT |
                VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SpritePushConstants),
                &push
            );

            vkCmdDraw(
                commandBuffer,
                6,
                1,
                0,
                0
            );
        }

        vkCmdEndRenderPass(commandBuffer);
    }

    void VulkanRenderer::RecordPresentPass(
        VkCommandBuffer commandBuffer,
        uint32_t imageIndex
    ) {
        VkClearValue clearColor{};
        clearColor.color = {
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType =
            VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass =
            m_PresentRenderPass.Get();
        renderPassInfo.framebuffer =
            m_PresentFramebuffer.GetFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {
            0,
            0
        };
        renderPassInfo.renderArea.extent =
            m_Swapchain.GetExtent();
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(
            commandBuffer,
            &renderPassInfo,
            VK_SUBPASS_CONTENTS_INLINE
        );

        m_FullscreenPass.Record(
            commandBuffer,
            m_OffscreenTarget.GetExtent(),
            m_Swapchain.GetExtent(),
            m_PixelConfig
        );

        vkCmdEndRenderPass(commandBuffer);
    }

}