#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanRenderPass {
    public:
        void InitColorDepth(
            VkDevice device,
            VkFormat colorFormat,
            VkFormat depthFormat,
            VkImageLayout colorFinalLayout
        );

        void InitPresent(
            VkDevice device,
            VkFormat colorFormat
        );

        void Shutdown();

        VkRenderPass Get() const;

    private:
        VkDevice m_Device = VK_NULL_HANDLE;
        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };

}