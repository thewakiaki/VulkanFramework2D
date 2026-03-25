#pragma once

class VulkanPhysicalDevice;

class VulkanLogicalDevice{

public:
    VulkanLogicalDevice();
    ~VulkanLogicalDevice();

    VulkanLogicalDevice(const VulkanLogicalDevice&) = delete;
    VulkanLogicalDevice& operator=(const VulkanLogicalDevice&) = delete;

    bool SetupLogicalDevice(const VulkanPhysicalDevice& pDevice);

    const VkDevice& GetLogicalDevice() const { return mLogicalDevice; }
    const VkQueue& GetGraphicsQueue() const { return mGraphicsQueue; }
    const VkQueue& GetPresentQueue() const { return mPresentQueue; }


private:
    void Cleanup();

    bool GetQueues(const VulkanPhysicalDevice& pDevice);

    void ViewEnabledDeviceExtensions(const char* const* enabled, uint32_t size);

    VkDevice mLogicalDevice = VK_NULL_HANDLE;

    VkQueue mGraphicsQueue = VK_NULL_HANDLE;
    VkQueue mPresentQueue = VK_NULL_HANDLE;
};
