#pragma once

class VulkanLogicalDevice;
class VulkanPhysicalDevice;

class VulkanCommandPool{

public:
    VulkanCommandPool(const VulkanLogicalDevice& lDevice, const VulkanPhysicalDevice& pDevice);
    ~VulkanCommandPool();

    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;

    bool SetupCommandPool();

    const VkCommandPool& GetCommandPool() const { return mCommandPool; }
    const VkCommandBuffer GetCommandBuffer() const { return mCommandBuffer; }

private:
    void Cleanup();

    VkCommandPool mCommandPool = VK_NULL_HANDLE;

    VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;

    const VulkanLogicalDevice& mLogicalDevice;
    const VulkanPhysicalDevice& mPhysicalDevice;

};
