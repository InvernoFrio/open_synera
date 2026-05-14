#pragma once

#include <vulkan/vulkan.h>
#include <vector>

struct GLFWwindow;

namespace Engine {

    class VulkanContext {
    public:
        void Init(GLFWwindow* window, bool enableValidation);
        void Shutdown();

        VkInstance GetInstance() const;
        VkSurfaceKHR GetSurface() const;

        bool IsValidationEnabled() const;

    private:
        void CreateInstance();
        void CreateSurface(GLFWwindow* window);
        void SetupDebugMessenger();

        std::vector<const char*> GetRequiredExtensions() const;
        bool CheckValidationLayerSupport() const;

    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;

        bool m_EnableValidation = false;
    };

}