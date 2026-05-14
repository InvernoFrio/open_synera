#include "Engine/Renderer/Vulkan/VulkanOffscreenRenderTarget.h"

namespace Engine {

    void VulkanOffscreenRenderTarget::Init(
        VkPhysicalDevice physicalDevice,
        VkDevice device,
        uint32_t width,
        uint32_t height
    ) {
        m_Device = device;

        m_Extent = {
            width,
            height
        };

        m_ColorImage.InitColorImage(
            physicalDevice,
            device,
            width,
            height,
            m_ColorFormat,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT,
            true
        );

        m_DepthImage.InitDepthImage(
            physicalDevice,
            device,
            width,
            height
        );

        m_RenderPass.InitColorDepth(
            device,
            m_ColorImage.GetFormat(),
            m_DepthImage.GetFormat(),
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        m_Framebuffer.InitSingleColorDepth(
            device,
            m_RenderPass.Get(),
            m_ColorImage.GetImageView(),
            m_DepthImage.GetImageView(),
            m_Extent
        );
    }

    void VulkanOffscreenRenderTarget::Shutdown() {
        m_Framebuffer.Shutdown();
        m_RenderPass.Shutdown();

        m_DepthImage.Shutdown();
        m_ColorImage.Shutdown();

        m_Extent = {};
    }

    VkExtent2D VulkanOffscreenRenderTarget::GetExtent() const {
        return m_Extent;
    }

    VkFormat VulkanOffscreenRenderTarget::GetColorFormat() const {
        return m_ColorImage.GetFormat();
    }

    VkFormat VulkanOffscreenRenderTarget::GetDepthFormat() const {
        return m_DepthImage.GetFormat();
    }

    VkImageView VulkanOffscreenRenderTarget::GetColorImageView() const {
        return m_ColorImage.GetImageView();
    }

    VkSampler VulkanOffscreenRenderTarget::GetColorSampler() const {
        return m_ColorImage.GetSampler();
    }

    VkRenderPass VulkanOffscreenRenderTarget::GetRenderPass() const {
        return m_RenderPass.Get();
    }

    VkFramebuffer VulkanOffscreenRenderTarget::GetFramebuffer() const {
        return m_Framebuffer.GetSingleFramebuffer();
    }

}