#include "VulkanInstance.h"
#include "Logs.h"

VulkanInstance::VulkanInstance(){

}

VulkanInstance::~VulkanInstance(){
    Cleanup();

}

bool VulkanInstance::SetupVulkanInstance(){

    Logs::Print("Setting Up Vulkan Instance");

    try {
        mVulkanInstance = vk::createInstance(SetInstanceCreateInfo());
        Logs::Print("Sucessfully Created Vulkan Instance");

        return true;

    } catch (const vk::SystemError& error) {
        Logs::LogError("Failed to create Vulkan Instance", error);

        return false;
    }

}

vk::InstanceCreateInfo VulkanInstance::SetInstanceCreateInfo(){
    vk::ApplicationInfo appInfo{
        .pApplicationName = mGameName,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = mEngineName,
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr
    };


    return createInfo;
}

void VulkanInstance::Cleanup(){

    if(mVulkanInstance)
    {
        mVulkanInstance.destroy(nullptr);
        mVulkanInstance = vk::Instance{};
    }
}
