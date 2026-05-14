#include "Engine/Renderer/Vulkan/VulkanImage.h"

#include <stdexcept>

namespace Engine {

    void VulkanImage::InitColorImage(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        bool createSampler
    ) {
        m_Device = device;
        m_Format = format;

        CreateImage(
            physicalDevice,
            device,
            width,
            height,
            format,
            usage,
            VK_IMAGE_ASPECT_COLOR_BIT
        );

        if (createSampler) {
            CreateNearestSampler();
        }
    }

    void VulkanImage::InitDepthImage(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t width,
        uint32_t height
    ) {
        m_Device = device;
        m_Format = FindDepthFormat(physicalDevice);

        CreateImage(
            physicalDevice,
            device,
            width,
            height,
            m_Format,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_IMAGE_ASPECT_DEPTH_BIT
        );
    }

    void VulkanImage::CreateImage(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t width,
        uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        VkImageAspectFlags aspectFlags
    ) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(
            device,
            &imageInfo,
            nullptr,
            &m_Image
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image");
        }

        VkMemoryRequirements memRequirements{};
        vkGetImageMemoryRequirements(
            device,
            m_Image,
            &memRequirements
        );

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(
            physicalDevice,
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        if (vkAllocateMemory(
            device,
            &allocInfo,
            nullptr,
            &m_Memory
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate image memory");
        }

        vkBindImageMemory(
            device,
            m_Image,
            m_Memory,
            0
        );

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_Image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;

        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(
            device,
            &viewInfo,
            nullptr,
            &m_ImageView
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image view");
        }
    }

    void VulkanImage::CreateNearestSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;

        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;

        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        if (vkCreateSampler(
            m_Device,
            &samplerInfo,
            nullptr,
            &m_Sampler
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create nearest sampler");
        }
    }

    void VulkanImage::Shutdown() {
        if (m_Sampler != VK_NULL_HANDLE) {
            vkDestroySampler(
                m_Device,
                m_Sampler,
                nullptr
            );

            m_Sampler = VK_NULL_HANDLE;
        }

        if (m_ImageView != VK_NULL_HANDLE) {
            vkDestroyImageView(
                m_Device,
                m_ImageView,
                nullptr
            );

            m_ImageView = VK_NULL_HANDLE;
        }

        if (m_Image != VK_NULL_HANDLE) {
            vkDestroyImage(
                m_Device,
                m_Image,
                nullptr
            );

            m_Image = VK_NULL_HANDLE;
        }

        if (m_Memory != VK_NULL_HANDLE) {
            vkFreeMemory(
                m_Device,
                m_Memory,
                nullptr
            );

            m_Memory = VK_NULL_HANDLE;
        }

        m_Format = VK_FORMAT_UNDEFINED;
    }

    VkImage VulkanImage::GetImage() const {
        return m_Image;
    }

    VkImageView VulkanImage::GetImageView() const {
        return m_ImageView;
    }

    VkSampler VulkanImage::GetSampler() const {
        return m_Sampler;
    }

    VkFormat VulkanImage::GetFormat() const {
        return m_Format;
    }

    VkFormat VulkanImage::FindDepthFormat(
        VkPhysicalDevice physicalDevice
    ) {
        VkFormat candidates[] = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };

        return FindSupportedFormat(
            physicalDevice,
            candidates,
            3,
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkFormat VulkanImage::FindSupportedFormat(
        VkPhysicalDevice physicalDevice,
        const VkFormat* candidates,
        uint32_t candidateCount,
        VkImageTiling tiling,
        VkFormatFeatureFlags features
    ) {
        for (uint32_t i = 0; i < candidateCount; i++) {
            VkFormat format = candidates[i];

            VkFormatProperties props{};
            vkGetPhysicalDeviceFormatProperties(
                physicalDevice,
                format,
                &props
            );

            if (tiling == VK_IMAGE_TILING_LINEAR &&
                (props.linearTilingFeatures & features) == features) {
                return format;
            }

            if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("Failed to find supported image format");
    }

    uint32_t VulkanImage::FindMemoryType(
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
            bool typeMatched =
                typeFilter & (1 << i);

            bool propertyMatched =
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties;

            if (typeMatched && propertyMatched) {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable memory type");
    }

}