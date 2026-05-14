#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace Engine {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool IsComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class VulkanDevice {
    public:
        void Init(VkInstance instance, VkSurfaceKHR surface);
        void Shutdown();

        VkPhysicalDevice GetPhysicalDevice() const;
        VkDevice GetDevice() const;

        VkQueue GetGraphicsQueue() const;
        VkQueue GetPresentQueue() const;

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;

    private:
        void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
        void CreateLogicalDevice();

        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const;

    private:
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;

        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
        VkQueue m_PresentQueue = VK_NULL_HANDLE;
    };

}