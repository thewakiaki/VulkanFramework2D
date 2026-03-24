#pragma once

#include "VulkanEnums.h"

class VulkanLogicalDevice;

class VulkanShaderModule{

public:
    VulkanShaderModule(const VulkanLogicalDevice& device, ShaderType type, std::string fileName);
    ~VulkanShaderModule();

    VulkanShaderModule(const VulkanShaderModule&) = delete;
    VulkanShaderModule& operator=(const VulkanShaderModule&) = delete;

    bool SetupShaderModule(const VulkanLogicalDevice& device);

    [[nodiscard]] const VkShaderModule& GetShaderModule() const { return mShaderModule; }

    const ShaderType& GetShaderType() const { return mShaderType; }

private:
    void Cleanup();

    bool GetShaderCode(const std::string& fileName);

    VkShaderModule mShaderModule = VK_NULL_HANDLE;

    std::string mFilename;

    std::vector<char> mShaderCode;
    ShaderType mShaderType;

    const VulkanLogicalDevice& mLogicalDevice;
};
