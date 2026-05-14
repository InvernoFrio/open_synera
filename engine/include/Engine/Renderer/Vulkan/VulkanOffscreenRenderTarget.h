#pragma once

#include "Engine/Renderer/Vulkan/VulkanFramebuffer.h"
#include "Engine/Renderer/Vulkan/VulkanImage.h"
#include "Engine/Renderer/Vulkan/VulkanRenderPass.h"

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanOffscreenRenderTarget {
    public:
        void Init(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            uint32_t width,
            uint32_t height
        );

        void Shutdown();

        VkExtent2D GetExtent() const;

        VkFormat GetColorFormat() const;
        VkFormat GetDepthFormat() const;

        VkImageView GetColorImageView() const;
        VkSampler GetColorSampler() const;

        VkRenderPass GetRenderPass() const;
        VkFramebuffer GetFramebuffer() const;

    private:
        VkDevice m_Device = VK_NULL_HANDLE;

        VkExtent2D m_Extent{};

        VkFormat m_ColorFormat = VK_FORMAT_R8G8B8A8_UNORM;

        VulkanImage m_ColorImage;
        VulkanImage m_DepthImage;

        VulkanRenderPass m_RenderPass;
        VulkanFramebuffer m_Framebuffer;
    };

}