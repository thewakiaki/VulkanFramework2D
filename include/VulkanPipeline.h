#pragma once

class VulkanLogicalDevice;
class VulkanShaderModule;


class VulkanPipeline{

public:
   VulkanPipeline(const VulkanLogicalDevice& mLogicalDevice);
   ~VulkanPipeline();

   VulkanPipeline(const VulkanPipeline&) = delete;
   VulkanPipeline& operator=(const VulkanPipeline&) = delete;

   bool SetupPipeline();

private:
    void Cleanup();

    bool SetShaderModules();

    void SetShaderStageCreateInfo();

    VkPipeline mPipeline;

    std::vector<std::unique_ptr<VulkanShaderModule>> mShaderModules;

    const VulkanLogicalDevice& mLogicalDevice;

    std::vector<VkPipelineShaderStageCreateInfo> mShaderStageCreateInfos;
};
