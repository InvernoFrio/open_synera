#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanPipeline {
    public:
        void Init(
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkDescriptorSetLayout descriptorSetLayout,
            const char* vertexShaderPath,
            const char* fragmentShaderPath
        );

        void Shutdown();

        VkPipeline GetPipeline() const;
        VkPipelineLayout GetPipelineLayout() const;

    private:
        VkDevice m_Device = VK_NULL_HANDLE;

        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_Pipeline = VK_NULL_HANDLE;
    };

}