#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace Engine {

    class VulkanShader {
    public:
        static std::vector<char> ReadFile(const std::string& path);

        static VkShaderModule CreateShaderModule(
            VkDevice device,
            const std::vector<char>& code
        );
    };

}