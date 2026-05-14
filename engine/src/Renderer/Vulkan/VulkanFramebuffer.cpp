#include "Engine/Renderer/Vulkan/VulkanFramebuffer.h"

#include <array>
#include <stdexcept>

namespace Engine {

    void VulkanFramebuffer::Init(
        VkDevice device,
        VkRenderPass renderPass,
        const std::vector<VkImageView>& swapchainImageViews,
        VkImageView depthImageView,
        VkExtent2D extent
    ) {
        m_Device = device;

        m_Framebuffers.resize(
            swapchainImageViews.size()
        );

        for (size_t i = 0; i < swapchainImageViews.size(); i++) {
            std::array<VkImageView, 2> attachments = {
                swapchainImageViews[i],
                depthImageView
            };

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = renderPass;
            createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            createInfo.pAttachments = attachments.data();
            createInfo.width = extent.width;
            createInfo.height = extent.height;
            createInfo.layers = 1;

            if (vkCreateFramebuffer(
                m_Device,
                &createInfo,
                nullptr,
                &m_Framebuffers[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create framebuffer");
            }
        }
    }

    void VulkanFramebuffer::Shutdown() {
        for (VkFramebuffer framebuffer : m_Framebuffers) {
            vkDestroyFramebuffer(
                m_Device,
                framebuffer,
                nullptr
            );
        }

        m_Framebuffers.clear();
    }

    const std::vector<VkFramebuffer>& VulkanFramebuffer::GetFramebuffers() const {
        return m_Framebuffers;
    }

}