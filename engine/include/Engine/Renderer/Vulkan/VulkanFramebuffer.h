#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine {

    class VulkanFramebuffer {
    public:
        void Init(
            VkDevice device,
            VkRenderPass renderPass,
            const std::vector<VkImageView>& swapchainImageViews,
            VkImageView depthImageView,
            VkExtent2D extent
        );

        void Shutdown();

        const std::vector<VkFramebuffer>& GetFramebuffers() const;

    private:
        VkDevice m_Device = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> m_Framebuffers;
    };

}