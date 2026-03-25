#include "VulkanLogicalDevice.h"

#include "Logs.h"
#include "VulkanStructs.h"
#include "VulkanPhysicalDevice.h"

VulkanLogicalDevice::VulkanLogicalDevice(){

}

VulkanLogicalDevice::~VulkanLogicalDevice(){
    Cleanup();
}

bool VulkanLogicalDevice::SetupLogicalDevice(const VulkanPhysicalDevice& pDevice){

    Logs::Print("--------------------");
    Logs::Print("Setting up Logical Device");

    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extDynState{
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
          .pNext = nullptr,
          .extendedDynamicState = VK_TRUE,
    };

    VkPhysicalDeviceVulkan11Features vk11Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = &extDynState,
        .shaderDrawParameters = VK_TRUE
    };

    VkPhysicalDeviceVulkan13Features vk13Features{
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
          .pNext = &vk11Features,
          .synchronization2 = VK_TRUE,
          .dynamicRendering = VK_TRUE
    };

    VkPhysicalDeviceFeatures2 features2{
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
          .pNext = &vk13Features,
    };

    features2.features.samplerAnisotropy = VK_TRUE;
    //features2.features.tessellationShader = VK_TRUE;
    features2.features.geometryShader = VK_TRUE;


    VkDeviceCreateInfo mDeviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &features2,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(pDevice.GetQueueCreateInfo().size()),
        .pQueueCreateInfos = pDevice.GetQueueCreateInfo().data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(pDevice.GetRequiredExtensions().size()),
        .ppEnabledExtensionNames = pDevice.GetRequiredExtensions().data(),
        .pEnabledFeatures = nullptr
    };

    ViewEnabledDeviceExtensions(mDeviceCreateInfo.ppEnabledExtensionNames, mDeviceCreateInfo.enabledExtensionCount);

    VkResult result = vkCreateDevice(pDevice.GetSelectedDevice().device, &mDeviceCreateInfo, nullptr, &mLogicalDevice);

    if(result == VK_SUCCESS)
    {
        Logs::Print("Logical Device Created");
        Logs::Print("--------------------");

        return GetQueues(pDevice);
        Logs::Print("--------------------");
    }

    Logs::PrintError("Failed to create Logicla Device", result);
    Logs::Print("--------------------");

    return false;
}

bool VulkanLogicalDevice::GetQueues(const VulkanPhysicalDevice& pDevice){

    vkGetDeviceQueue(mLogicalDevice, static_cast<uint32_t>(pDevice.GetSelectedDevice().graphicsQueueIndex), 0, &mGraphicsQueue);

    if(mGraphicsQueue == VK_NULL_HANDLE){
        Logs::Print("Failed to Retrieve Graphics Queue");
        return false;
    }

    Logs::Print("Graphics Queue Retrieved");

    vkGetDeviceQueue(mLogicalDevice, static_cast<uint32_t>(pDevice.GetSelectedDevice().presentQueueIndex), 0, &mPresentQueue);

    if(mPresentQueue == VK_NULL_HANDLE){
        Logs::Print("Failed to Retrieve Present Queue");
        return false;
    }

    Logs::Print("Present Queue Retrieved");

    return true;
}

void VulkanLogicalDevice::ViewEnabledDeviceExtensions(const char* const* enabled, uint32_t size){

    Logs::Print("--------------------");
    Logs::Print("Enabled Device Extensions");
    Logs::Print("--------------------");

    for(int i = 0; i < size; ++i)
    {
        Logs::Print(enabled[i]);
    }
    Logs::Print("--------------------");

}

void VulkanLogicalDevice::Cleanup(){

    if(mLogicalDevice != VK_NULL_HANDLE){
        vkDestroyDevice(mLogicalDevice, nullptr);
        mLogicalDevice = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Logical Device");
    }

}
