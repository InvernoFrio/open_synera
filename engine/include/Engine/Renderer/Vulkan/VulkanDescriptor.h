#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanDescriptor {
    public:
        void Init(
            VkDevice device,
            VkBuffer cameraBuffer,
            VkDeviceSize cameraBufferSize
        );

        void Shutdown();

        VkDescriptorSetLayout GetLayout() const;
        VkDescriptorSet GetSet() const;

    private:
        void CreateDescriptorSetLayout();
        void CreateDescriptorPool();
        void CreateDescriptorSet(
            VkBuffer cameraBuffer,
            VkDeviceSize cameraBufferSize
        );

    private:
        VkDevice m_Device = VK_NULL_HANDLE;

        VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
        VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
    };

}