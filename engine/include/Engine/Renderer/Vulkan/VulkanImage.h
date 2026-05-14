#pragma once

#include <vulkan/vulkan.h>

namespace Engine {

    class VulkanImage {
    public:
        void InitColorImage(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            uint32_t width,
            uint32_t height,
            VkFormat format,
            VkImageUsageFlags usage,
            bool createSampler
        );

        void InitDepthImage(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            uint32_t width,
            uint32_t height
        );

        void Shutdown();

        VkImage GetImage() const;
        VkImageView GetImageView() const;
        VkSampler GetSampler() const;
        VkFormat GetFormat() const;

        static VkFormat FindDepthFormat(
            VkPhysicalDevice physicalDevice
        );

    private:
        void CreateImage(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            uint32_t width,
            uint32_t height,
            VkFormat format,
            VkImageUsageFlags usage,
            VkImageAspectFlags aspectFlags
        );

        void CreateNearestSampler();

        static VkFormat FindSupportedFormat(
            VkPhysicalDevice physicalDevice,
            const VkFormat* candidates,
            uint32_t candidateCount,
            VkImageTiling tiling,
            VkFormatFeatureFlags features
        );

        uint32_t FindMemoryType(
            VkPhysicalDevice physicalDevice,
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        );

    private:
        VkDevice m_Device = VK_NULL_HANDLE;

        VkImage m_Image = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        VkImageView m_ImageView = VK_NULL_HANDLE;
        VkSampler m_Sampler = VK_NULL_HANDLE;

        VkFormat m_Format = VK_FORMAT_UNDEFINED;
    };

}