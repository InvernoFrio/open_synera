#include "Engine/Renderer/Vulkan/VulkanSwapchain.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"

#include <algorithm>
#include <array>
#include <stdexcept>

namespace Engine {

    void VulkanSwapchain::Init(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        VkSurfaceKHR surface,
        uint32_t width,
        uint32_t height,
        const QueueFamilyIndices& queueFamilyIndices,
        VkSwapchainKHR oldSwapchain
    ) {
        m_Device = device;

        SwapchainSupportDetails support =
            QuerySupport(
                physicalDevice,
                surface
            );

        VkSurfaceFormatKHR surfaceFormat =
            ChooseSurfaceFormat(
                support.formats
            );

        VkPresentModeKHR presentMode =
            ChoosePresentMode(
                support.presentModes
            );

        VkExtent2D extent =
            ChooseExtent(
                support.capabilities,
                width,
                height
            );

        uint32_t imageCount =
            support.capabilities.minImageCount + 1;

        if (support.capabilities.maxImageCount > 0 &&
            imageCount > support.capabilities.maxImageCount) {
            imageCount = support.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType =
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;

        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;

        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;

        createInfo.imageUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndicesRaw[] = {
            queueFamilyIndices.graphicsFamily.value(),
            queueFamilyIndices.presentFamily.value()
        };

        if (queueFamilyIndices.graphicsFamily !=
            queueFamilyIndices.presentFamily) {
            createInfo.imageSharingMode =
                VK_SHARING_MODE_CONCURRENT;

            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices =
                queueFamilyIndicesRaw;
        }
        else {
            createInfo.imageSharingMode =
                VK_SHARING_MODE_EXCLUSIVE;

            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform =
            support.capabilities.currentTransform;

        createInfo.compositeAlpha =
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = oldSwapchain;

        if (vkCreateSwapchainKHR(
            device,
            &createInfo,
            nullptr,
            &m_Swapchain
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swapchain");
        }

        vkGetSwapchainImagesKHR(
            device,
            m_Swapchain,
            &imageCount,
            nullptr
        );

        m_Images.resize(imageCount);

        vkGetSwapchainImagesKHR(
            device,
            m_Swapchain,
            &imageCount,
            m_Images.data()
        );

        m_ImageFormat = surfaceFormat.format;
        m_Extent = extent;

        m_ImageViews.resize(m_Images.size());

        for (size_t i = 0; i < m_Images.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType =
                VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

            viewInfo.image = m_Images[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_ImageFormat;

            viewInfo.subresourceRange.aspectMask =
                VK_IMAGE_ASPECT_COLOR_BIT;

            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;

            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(
                device,
                &viewInfo,
                nullptr,
                &m_ImageViews[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create swapchain image view");
            }
        }
    }

    void VulkanSwapchain::Shutdown() {
        for (VkImageView imageView : m_ImageViews) {
            vkDestroyImageView(
                m_Device,
                imageView,
                nullptr
            );
        }

        m_ImageViews.clear();
        m_Images.clear();

        if (m_Swapchain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(
                m_Device,
                m_Swapchain,
                nullptr
            );

            m_Swapchain = VK_NULL_HANDLE;
        }

        m_ImageFormat = VK_FORMAT_UNDEFINED;
        m_Extent = {};
    }

    VkSwapchainKHR VulkanSwapchain::GetSwapchain() const {
        return m_Swapchain;
    }

    VkFormat VulkanSwapchain::GetImageFormat() const {
        return m_ImageFormat;
    }

    VkExtent2D VulkanSwapchain::GetExtent() const {
        return m_Extent;
    }

    const std::vector<VkImageView>& VulkanSwapchain::GetImageViews() const {
        return m_ImageViews;
    }

    SwapchainSupportDetails VulkanSwapchain::QuerySupport(
        VkPhysicalDevice device,
        VkSurfaceKHR surface
    ) {
        SwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device,
            surface,
            &details.capabilities
        );

        uint32_t formatCount = 0;

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device,
            surface,
            &formatCount,
            nullptr
        );

        if (formatCount > 0) {
            details.formats.resize(formatCount);

            vkGetPhysicalDeviceSurfaceFormatsKHR(
                device,
                surface,
                &formatCount,
                details.formats.data()
            );
        }

        uint32_t presentModeCount = 0;

        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            surface,
            &presentModeCount,
            nullptr
        );

        if (presentModeCount > 0) {
            details.presentModes.resize(presentModeCount);

            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                surface,
                &presentModeCount,
                details.presentModes.data()
            );
        }

        return details;
    }

    VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& formats
    ) {
        for (const auto& available : formats) {
            if (available.format == VK_FORMAT_B8G8R8A8_SRGB &&
                available.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return available;
            }
        }

        return formats[0];
    }

    VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(
        const std::vector<VkPresentModeKHR>& presentModes
    ) {
        for (const auto& available : presentModes) {
            if (available == VK_PRESENT_MODE_MAILBOX_KHR) {
                return available;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::ChooseExtent(
        const VkSurfaceCapabilitiesKHR& capabilities,
        uint32_t width,
        uint32_t height
    ) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }

        VkExtent2D actualExtent{
            width,
            height
        };

        actualExtent.width =
            std::clamp(
                actualExtent.width,
                capabilities.minImageExtent.width,
                capabilities.maxImageExtent.width
            );

        actualExtent.height =
            std::clamp(
                actualExtent.height,
                capabilities.minImageExtent.height,
                capabilities.maxImageExtent.height
            );

        return actualExtent;
    }

}