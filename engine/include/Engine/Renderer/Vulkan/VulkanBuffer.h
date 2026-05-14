#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanBuffer {
    public:
        void Init(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        );

        void Shutdown();

        void UploadData(const void* data, VkDeviceSize size);

        VkBuffer GetBuffer() const;
        VkDeviceSize GetSize() const;

    private:
        uint32_t FindMemoryType(
            VkPhysicalDevice physicalDevice,
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );

    private:
        VkDevice m_Device = VK_NULL_HANDLE;
        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        VkDeviceSize m_Size = 0;
    };

}