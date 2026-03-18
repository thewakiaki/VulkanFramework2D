#pragma once

class VulkanInstance{

public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

    bool SetupVulkanInstance();

private:
    void Cleanup();

    vk::InstanceCreateInfo SetInstanceCreateInfo();

    vk::Instance mVulkanInstance;

    const char* mGameName = "Game Name";
    const char* mEngineName = "Engine Name";
};
