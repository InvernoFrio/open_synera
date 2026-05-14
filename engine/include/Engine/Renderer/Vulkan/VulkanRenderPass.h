#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanRenderPass {
    public:
        void Init(
            VkDevice device,
            VkFormat colorFormat,
            VkFormat depthFormat
        );

        void Shutdown();

        VkRenderPass Get() const;

    private:
        VkDevice m_Device = VK_NULL_HANDLE;
        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };

}