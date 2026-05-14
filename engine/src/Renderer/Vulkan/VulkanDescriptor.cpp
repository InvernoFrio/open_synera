#include "Engine/Renderer/Vulkan/VulkanDescriptor.h"

#include <stdexcept>

namespace Engine {

    void VulkanDescriptor::Init(
        VkDevice device,
        VkBuffer cameraBuffer,
        VkDeviceSize cameraBufferSize
    ) {
        m_Device = device;

        CreateDescriptorSetLayout();
        CreateDescriptorPool();
        CreateDescriptorSet(
            cameraBuffer,
            cameraBufferSize
        );
    }

    void VulkanDescriptor::Shutdown() {
        if (m_DescriptorPool != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(
                m_Device,
                m_DescriptorPool,
                nullptr
            );

            m_DescriptorPool = VK_NULL_HANDLE;
        }

        if (m_DescriptorSetLayout != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(
                m_Device,
                m_DescriptorSetLayout,
                nullptr
            );

            m_DescriptorSetLayout = VK_NULL_HANDLE;
        }

        m_DescriptorSet = VK_NULL_HANDLE;
    }

    VkDescriptorSetLayout VulkanDescriptor::GetLayout() const {
        return m_DescriptorSetLayout;
    }

    VkDescriptorSet VulkanDescriptor::GetSet() const {
        return m_DescriptorSet;
    }

    void VulkanDescriptor::CreateDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding cameraBinding{};
        cameraBinding.binding = 0;
        cameraBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        cameraBinding.descriptorCount = 1;
        cameraBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        cameraBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &cameraBinding;

        if (vkCreateDescriptorSetLayout(
            m_Device,
            &layoutInfo,
            nullptr,
            &m_DescriptorSetLayout
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout");
        }
    }

    void VulkanDescriptor::CreateDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = 1;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(
            m_Device,
            &poolInfo,
            nullptr,
            &m_DescriptorPool
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool");
        }
    }

    void VulkanDescriptor::CreateDescriptorSet(
        VkBuffer cameraBuffer,
        VkDeviceSize cameraBufferSize
    ) {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_DescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_DescriptorSetLayout;

        if (vkAllocateDescriptorSets(
            m_Device,
            &allocInfo,
            &m_DescriptorSet
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate descriptor set");
        }

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = cameraBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = cameraBufferSize;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_DescriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(
            m_Device,
            1,
            &descriptorWrite,
            0,
            nullptr
        );
    }

}