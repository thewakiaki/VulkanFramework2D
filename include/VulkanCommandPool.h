#pragma once

#include "VulkanStructs.h"

class VulkanLogicalDevice;
class VulkanPhysicalDevice;
class VulkanSwapchain;
class VulkanPipeline;

class VulkanCommandPool{

public:
    VulkanCommandPool(const VulkanLogicalDevice& lDevice, const VulkanPhysicalDevice& pDevice, const VulkanSwapchain& swapchain,
                        const VulkanPipeline& pipeline);
    ~VulkanCommandPool();

    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;

    bool SetupCommandPool();
    bool SetupCommandBuffer();
    bool RecordCommandBuffer(uint32_t imageIndex);

    const VkCommandPool& GetCommandPool() const { return mCommandPool; }
    const VkCommandBuffer GetCommandBuffer() const { return mCommandBuffer; }

private:
    void Cleanup();

    VkCommandBufferBeginInfo SetCommandBeginInfo();
    VkRenderingInfo SetRenderingInfo(uint32_t imageIndex);
    VkRenderingAttachmentInfo SetRenderAttachmentInfo(uint32_t imageIndex);

    void TransitionImageLayout(uint32_t imageIndex, VulkanStructs::ImageTransition& imageTransition);

    void UpdateViewport();
    void UpdateScissor();

    VkCommandPool mCommandPool = VK_NULL_HANDLE;

    VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;

    VulkanStructs::ImageTransition mStartImageTransition;
    VulkanStructs::ImageTransition mEndImageTransition;

    VkViewport mViewport{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    VkRect2D mScissor{{0, 0}, {}};

    const VulkanLogicalDevice& mLogicalDevice;
    const VulkanPhysicalDevice& mPhysicalDevice;
    const VulkanSwapchain& mSwapchain;
    const VulkanPipeline& mPipeline;

};
