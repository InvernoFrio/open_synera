#include "Engine/Renderer/Vulkan/VulkanSpritePipeline.h"
#include "Engine/Renderer/Vulkan/VulkanShader.h"

#include <stdexcept>

namespace Engine {

    void VulkanSpritePipeline::Init(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkDescriptorSetLayout descriptorSetLayout
    ) {
        m_Device = device;

        auto vertCode =
            VulkanShader::ReadFile(
                "assets/shaders/sprite.vert.spv"
            );

        auto fragCode =
            VulkanShader::ReadFile(
                "assets/shaders/sprite.frag.spv"
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
        vertStage.stage =
            VK_SHADER_STAGE_VERTEX_BIT;
        vertStage.module =
            vertShaderModule;
        vertStage.pName =
            "main";

        VkPipelineShaderStageCreateInfo fragStage{};
        fragStage.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragStage.stage =
            VK_SHADER_STAGE_FRAGMENT_BIT;
        fragStage.module =
            fragShaderModule;
        fragStage.pName =
            "main";

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

        /*
            Sprite 使用深度测试，但不写入深度。
            这样：
            - 被棋子 / 棋盘挡住的特效不会乱穿
            - 多个 sprite 不会互相污染 depth
        */
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_FALSE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        /*
            Alpha Blend:
                out = src * srcAlpha + dst * (1 - srcAlpha)
        */
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;

        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor =
            VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp =
            VK_BLEND_OP_ADD;

        colorBlendAttachment.srcAlphaBlendFactor =
            VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.alphaBlendOp =
            VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments =
            &colorBlendAttachment;

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT |
            VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;

        /*
            SpritePushConstants:
                vec4 positionSize
                vec4 color
                vec4 params

            total = 12 floats = 48 bytes
        */
        pushConstantRange.size =
            sizeof(float) * 12;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts =
            &descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges =
            &pushConstantRange;

        if (vkCreatePipelineLayout(
            m_Device,
            &pipelineLayoutInfo,
            nullptr,
            &m_PipelineLayout
        ) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to create sprite pipeline layout"
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
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;

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
                "Failed to create sprite pipeline"
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

    void VulkanSpritePipeline::Shutdown() {
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
    }

    VkPipeline VulkanSpritePipeline::GetPipeline() const {
        return m_Pipeline;
    }

    VkPipelineLayout VulkanSpritePipeline::GetPipelineLayout() const {
        return m_PipelineLayout;
    }

}