#pragma once

#include "VulkanStructs.h"

class VulkanInstance{

public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

    bool SetupVulkanInstance();

private:
    void Cleanup();

    bool SetupExtensions();

    vk::InstanceCreateInfo SetInstanceCreateInfo();

    void ViewFlaggedExtensions();

    vk::Instance mVulkanInstance;

    const char* mGameName = "Game Name";
    const char* mEngineName = "Engine Name";

    uint32_t mVkExtensionCount = 0;
    std::vector<VulkanStructs::ExtensionFlags> mExtensionFlags;
    std::vector<VkExtensionProperties> mAvailableExtensions;
    std::vector<const char*> mRequiredExtensions;

    const std::vector<char const*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };

};
