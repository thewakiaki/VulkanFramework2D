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

    [[nodiscard]] const VkPhysicalDevice& GetSelectedDevice() const { return mSelectedDevice; }

private:
    bool GetDeviceScores(const VulkanInstance& instance);

    bool CheckSuitable(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features);
    uint32_t ScoreDevice(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features);

    void SelectBestDevice();

    void Cleanup();

    VkPhysicalDevice mSelectedDevice = VK_NULL_HANDLE;

    std::vector<VulkanStructs::PhysicalDeviceDetails> mAvailableDevices;

    uint32_t mPhyiscalDeviceCount = 0;

};
