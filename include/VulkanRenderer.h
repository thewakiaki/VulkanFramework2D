#pragma once

class VulkanLogicalDevice;
class VulkanSwapchain;
class VulkanCommandPool;

class VulkanRenderer{

public:
    VulkanRenderer(const VulkanLogicalDevice& lDevice, const VulkanSwapchain& swapchain, VulkanCommandPool& cmdPool);
    ~VulkanRenderer();

    VulkanRenderer(const VulkanRenderer&) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;

    bool SetupRenderer();

    void DrawFrame();

private:
    void Cleanup();

    bool SetupSyncObjects();
    bool SetupSemaphores();
    bool SetupFences();

    VkSubmitInfo SetSubmitInfo(VkPipelineStageFlags* waitStages);
    VkPresentInfoKHR SetPresentInfo(uint32_t imageIndex);

    VkSemaphore mRenderStartSemaphore = VK_NULL_HANDLE;
    VkSemaphore mRenderFinishSemaphore = VK_NULL_HANDLE;
    VkFence mDrawFence = VK_NULL_HANDLE;

    const VulkanLogicalDevice& mLogicalDevice;
    const VulkanSwapchain& mSwapchain;
    VulkanCommandPool& mCommandPool;
};
