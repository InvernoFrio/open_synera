#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanFullscreenPass {
    public:
        void Init(
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkImageView sourceImageView,
            VkSampler sourceSampler
        );

        void Shutdown();

        void Record(
            VkCommandBuffer commandBuffer
        );

    private:
        void CreateDescriptorSetLayout();
        void CreateDescriptorPool();
        void CreateDescriptorSet(
            VkImageView sourceImageView,
            VkSampler sourceSampler
        );

        void CreatePipeline(
            VkExtent2D extent,
            VkRenderPass renderPass
        );

    private:
        VkDevice m_Device = VK_NULL_HANDLE;

        VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
        VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;

        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_Pipeline = VK_NULL_HANDLE;
    };

}