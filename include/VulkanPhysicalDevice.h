#pragma once

#include "VulkanStructs.h"

class VulkanInstance;

class VulkanPhysicalDevice{

public:
    VulkanPhysicalDevice();
    ~VulkanPhysicalDevice();

    VulkanPhysicalDevice(const VulkanInstance&) = delete;
    VulkanPhysicalDevice& operator=(const VulkanInstance&) = delete;

    bool SetupPhysicalDevice(const VulkanInstance& instance);

    [[nodiscard]] const VulkanStructs::PhysicalDeviceDetails& GetSelectedDevice() const { return mSelectedDevice; }
    [[nodiscard]] const std::vector<VkDeviceQueueCreateInfo>& GetQueueCreateInfo() const { return mQueueCreateInfos; }


private:
    bool GetDeviceScores(const VulkanInstance& instance);

    bool CheckSuitable(const VkPhysicalDevice& device, const VkPhysicalDeviceProperties& properties);
    uint32_t ScoreDevice(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features);
    void SelectBestDevice();

    bool CheckQueueSupport(const VkPhysicalDevice& device, size_t& graphicsQueueIndex, size_t& presentQueueIndex);
    bool CheckExtensionSupport(const VkPhysicalDevice& device);
    bool CheckDeviceProperties(const VkPhysicalDeviceProperties& properties);
    bool CheckDeviceFeatures(const VkPhysicalDevice& device);

    void SetQueues();

    VulkanStructs::PhysicalDeviceDetails mSelectedDevice;

    uint32_t mPhyiscalDeviceCount = 0;
    std::vector<VulkanStructs::PhysicalDeviceDetails> mAvailableDevices;

    std::vector<const char*> mRequiredExtensions;

    VkQueue mGraphicsQueue;
    VkQueue mPresentQueue;
    std::vector<VkDeviceQueueCreateInfo> mQueueCreateInfos;

};
