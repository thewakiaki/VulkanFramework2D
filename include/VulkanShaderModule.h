#pragma once

#include "VulkanEnums.h"

class VulkanLogicalDevice;

class VulkanShaderModule{

public:
    VulkanShaderModule(const VulkanLogicalDevice& device);
    ~VulkanShaderModule();

    VulkanShaderModule(const VulkanShaderModule&) = delete;
    VulkanShaderModule& operator=(const VulkanShaderModule&) = delete;

    bool SetupShaderModule(const VulkanLogicalDevice& device, const std::string& fileName, ShaderType type);

    [[nodiscard]] const VkShaderModule& GetShaderModule() const { return mShaderModule; }

private:
    void Cleanup();

    bool GetShaderCode(const std::string& fileName);

    VkShaderModule mShaderModule = VK_NULL_HANDLE;

    std::vector<char> mShaderCode;
    ShaderType mShaderType;

    const VulkanLogicalDevice& mLogicalDevice;
};
