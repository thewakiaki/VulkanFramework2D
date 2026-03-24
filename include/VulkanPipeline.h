#pragma once

class VulkanLogicalDevice;
class VulkanShaderModule;
class VulkanSwapchain;


class VulkanPipeline{

public:
   VulkanPipeline(const VulkanLogicalDevice& mLogicalDevice, const VulkanSwapchain& swapchain);
   ~VulkanPipeline();

   VulkanPipeline(const VulkanPipeline&) = delete;
   VulkanPipeline& operator=(const VulkanPipeline&) = delete;

   bool SetupPipeline();

   const VkPipeline& GetPipeline() const { return mPipeline; }

private:
    void Cleanup();

    bool SetShaderModules();

    void SetShaderStageCreateInfo();

    VkPipelineDynamicStateCreateInfo SetDynamicStateCreateInfo();
    VkPipelineVertexInputStateCreateInfo SetVertexInputCreateInfo();
    VkPipelineInputAssemblyStateCreateInfo SetInputAssemblyCreateInfo();
    VkPipelineViewportStateCreateInfo SetViewportCreateInfo();
    VkPipelineRasterizationStateCreateInfo SetRasterCreateInfo();
    VkPipelineMultisampleStateCreateInfo SetMultiSampleCreateInfo();
    VkPipelineDepthStencilStateCreateInfo SetDepthStencilCreateInfo();
    VkPipelineColorBlendStateCreateInfo SetColorBlendCreateInfo();
    VkPipelineLayoutCreateInfo SetLayoutCreateInfo();
    VkPipelineRenderingCreateInfo SetRenderCreateInfo();

    VkPipeline mPipeline = VK_NULL_HANDLE;
    VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

    VkViewport mViewport;
    VkRect2D mScissor;

    std::vector<std::unique_ptr<VulkanShaderModule>> mShaderModules;
    std::vector<VkDynamicState> mDynamicStates;

    const VulkanLogicalDevice& mLogicalDevice;
    const VulkanSwapchain& mSwapchain;

    std::vector<VkPipelineShaderStageCreateInfo> mShaderStageCreateInfos;
};
