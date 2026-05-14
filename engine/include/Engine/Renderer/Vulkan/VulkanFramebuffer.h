#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace Engine {

    class VulkanFramebuffer {
    public:
        void InitSwapchainColorOnly(
            VkDevice device,
            VkRenderPass renderPass,
            const std::vector<VkImageView>& swapchainImageViews,
            VkExtent2D extent
        );

        void InitSingleColorDepth(
            VkDevice device,
            VkRenderPass renderPass,
            VkImageView colorImageView,
            VkImageView depthImageView,
            VkExtent2D extent
        );

        void Shutdown();

        const std::vector<VkFramebuffer>& GetFramebuffers() const;
        VkFramebuffer GetSingleFramebuffer() const;

    private:
        VkDevice m_Device = VK_NULL_HANDLE;

        std::vector<VkFramebuffer> m_Framebuffers;
    };

}