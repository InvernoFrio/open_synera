#include "Engine/Renderer/Vulkan/VulkanBuffer.h"

#include <stdexcept>
#include <cstring>

namespace Engine {

    void VulkanBuffer::Init(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties
    ) {
        m_Device = device;
        m_Size = size;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(
            m_Device,
            &bufferInfo,
            nullptr,
            &m_Buffer
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create buffer");
        }

        VkMemoryRequirements memRequirements{};
        vkGetBufferMemoryRequirements(
            m_Device,
            m_Buffer,
            &memRequirements
        );

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(
            physicalDevice,
            memRequirements.memoryTypeBits,
            properties
        );

        if (vkAllocateMemory(
            m_Device,
            &allocInfo,
            nullptr,
            &m_Memory
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate buffer memory");
        }

        vkBindBufferMemory(
            m_Device,
            m_Buffer,
            m_Memory,
            0
        );
    }

    void VulkanBuffer::Shutdown() {
        if (m_Buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_Device, m_Buffer, nullptr);
            m_Buffer = VK_NULL_HANDLE;
        }

        if (m_Memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_Device, m_Memory, nullptr);
            m_Memory = VK_NULL_HANDLE;
        }

        m_Size = 0;
    }

    void VulkanBuffer::UploadData(const void* data, VkDeviceSize size) {
        void* mapped = nullptr;

        vkMapMemory(
            m_Device,
            m_Memory,
            0,
            size,
            0,
            &mapped
        );

        std::memcpy(mapped, data, static_cast<size_t>(size));

        vkUnmapMemory(m_Device, m_Memory);
    }

    VkBuffer VulkanBuffer::GetBuffer() const {
        return m_Buffer;
    }

    VkDeviceSize VulkanBuffer::GetSize() const {
        return m_Size;
    }

    uint32_t VulkanBuffer::FindMemoryType(
        VkPhysicalDevice physicalDevice,
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    ) {
        VkPhysicalDeviceMemoryProperties memProperties{};
        vkGetPhysicalDeviceMemoryProperties(
            physicalDevice,
            &memProperties
        );

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            bool typeMatched = typeFilter & (1 << i);

            bool propertyMatched =
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties;

            if (typeMatched && propertyMatched) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type");
    }

}