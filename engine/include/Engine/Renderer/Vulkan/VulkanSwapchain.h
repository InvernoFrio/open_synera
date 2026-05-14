#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Engine {

    struct QueueFamilyIndices;

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanSwapchain {
    public:
        void Init(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            VkSurfaceKHR surface,
            uint32_t width,
            uint32_t height,
            const QueueFamilyIndices& queueFamilyIndices,
            VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE
        );

        void Shutdown();

        VkSwapchainKHR GetSwapchain() const;
        VkFormat GetImageFormat() const;
        VkExtent2D GetExtent() const;

        const std::vector<VkImageView>& GetImageViews() const;

    private:
        SwapchainSupportDetails QuerySupport(
            VkPhysicalDevice device,
            VkSurfaceKHR surface
        );

        VkSurfaceFormatKHR ChooseSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& formats
        );

        VkPresentModeKHR ChoosePresentMode(
            const std::vector<VkPresentModeKHR>& presentModes
        );

        VkExtent2D ChooseExtent(
            const VkSurfaceCapabilitiesKHR& capabilities,
            uint32_t width,
            uint32_t height
        );

    private:
        VkDevice m_Device = VK_NULL_HANDLE;
        VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;

        VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D m_Extent{};
    };

}