#include "Engine/Renderer/Vulkan/VulkanFullscreenPass.h"
#include "Engine/Renderer/Vulkan/VulkanShader.h"

#include <stdexcept>

namespace Engine {

    void VulkanFullscreenPass::Init(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkImageView sourceImageView,
        VkSampler sourceSampler
    ) {
        m_Device = device;

        CreateDescriptorSetLayout();
        CreateDescriptorPool();

        CreateDescriptorSet(
            sourceImageView,
            sourceSampler
        );

        CreatePipeline(
            extent,
            renderPass
        );
    }

    void VulkanFullscreenPass::Shutdown() {
        if (m_Pipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(
                m_Device,
                m_Pipeline,
                nullptr
            );

            m_Pipeline = VK_NULL_HANDLE;
        }

        if (m_PipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(
                m_Device,
                m_PipelineLayout,
                nullptr
            );

            m_PipelineLayout = VK_NULL_HANDLE;
        }

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

    void VulkanFullscreenPass::CreateDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding imageBinding{};
        imageBinding.binding = 0;
        imageBinding.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imageBinding.descriptorCount = 1;
        imageBinding.stageFlags =
            VK_SHADER_STAGE_FRAGMENT_BIT;
        imageBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &imageBinding;

        if (vkCreateDescriptorSetLayout(
            m_Device,
            &layoutInfo,
            nullptr,
            &m_DescriptorSetLayout
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create fullscreen descriptor set layout"
            );
        }
    }

    void VulkanFullscreenPass::CreateDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = 1;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(
            m_Device,
            &poolInfo,
            nullptr,
            &m_DescriptorPool
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create fullscreen descriptor pool"
            );
        }
    }

    void VulkanFullscreenPass::CreateDescriptorSet(
        VkImageView sourceImageView,
        VkSampler sourceSampler
    ) {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_DescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_DescriptorSetLayout;

        if (vkAllocateDescriptorSets(
            m_Device,
            &allocInfo,
            &m_DescriptorSet
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to allocate fullscreen descriptor set"
            );
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout =
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = sourceImageView;
        imageInfo.sampler = sourceSampler;

        VkWriteDescriptorSet write{};
        write.sType =
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_DescriptorSet;
        write.dstBinding = 0;
        write.dstArrayElement = 0;
        write.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(
            m_Device,
            1,
            &write,
            0,
            nullptr
        );
    }

    void VulkanFullscreenPass::CreatePipeline(
        VkExtent2D extent,
        VkRenderPass renderPass
    ) {
        auto vertCode =
            VulkanShader::ReadFile(
                "assets/shaders/fullscreen.vert.spv"
            );

        auto fragCode =
            VulkanShader::ReadFile(
                "assets/shaders/pixel_resolve.frag.spv"
            );

        VkShaderModule vertShaderModule =
            VulkanShader::CreateShaderModule(
                m_Device,
                vertCode
            );

        VkShaderModule fragShaderModule =
            VulkanShader::CreateShaderModule(
                m_Device,
                fragCode
            );

        VkPipelineShaderStageCreateInfo vertStage{};
        vertStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertStage.module = vertShaderModule;
        vertStage.pName = "main";

        VkPipelineShaderStageCreateInfo fragStage{};
        fragStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragStage.module = fragShaderModule;
        fragStage.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {
            vertStage,
            fragStage
        };

        VkPipelineVertexInputStateCreateInfo vertexInput{};
        vertexInput.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInput.vertexBindingDescriptionCount = 0;
        vertexInput.pVertexBindingDescriptions = nullptr;
        vertexInput.vertexAttributeDescriptionCount = 0;
        vertexInput.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology =
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        /*
            这里给一个默认 viewport/scissor。
            真正的 viewport/scissor 在 Record() 里动态设置。
        */
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width =
            static_cast<float>(extent.width);
        viewport.height =
            static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {
            0,
            0
        };
        scissor.extent = extent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode =
            VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode =
            VK_CULL_MODE_NONE;
        rasterizer.frontFace =
            VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples =
            VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments =
            &colorBlendAttachment;

        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts =
            &m_DescriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(
            m_Device,
            &pipelineLayoutInfo,
            nullptr,
            &m_PipelineLayout
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create fullscreen pipeline layout"
            );
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType =
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;

        pipelineInfo.pVertexInputState = &vertexInput;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;

        pipelineInfo.layout = m_PipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(
            m_Device,
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &m_Pipeline
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create fullscreen pipeline"
            );
        }

        vkDestroyShaderModule(
            m_Device,
            fragShaderModule,
            nullptr
        );

        vkDestroyShaderModule(
            m_Device,
            vertShaderModule,
            nullptr
        );
    }

    void VulkanFullscreenPass::Record(
        VkCommandBuffer commandBuffer,
        VkExtent2D sourceExtent,
        VkExtent2D targetExtent,
        const PixelRenderConfig& config
    ) {
        PixelViewport pixelViewport =
            CalculatePixelViewport(
                config,
                targetExtent.width,
                targetExtent.height
            );

        if (pixelViewport.width == 0 ||
            pixelViewport.height == 0) {
            return;
        }

        VkViewport viewport{};
        viewport.x =
            static_cast<float>(pixelViewport.x);
        viewport.y =
            static_cast<float>(pixelViewport.y);
        viewport.width =
            static_cast<float>(pixelViewport.width);
        viewport.height =
            static_cast<float>(pixelViewport.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {
            pixelViewport.x,
            pixelViewport.y
        };

        scissor.extent = {
            pixelViewport.width,
            pixelViewport.height
        };

        vkCmdSetViewport(
            commandBuffer,
            0,
            1,
            &viewport
        );

        vkCmdSetScissor(
            commandBuffer,
            0,
            1,
            &scissor
        );

        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_Pipeline
        );

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_PipelineLayout,
            0,
            1,
            &m_DescriptorSet,
            0,
            nullptr
        );

        vkCmdDraw(
            commandBuffer,
            3,
            1,
            0,
            0
        );
    }

}