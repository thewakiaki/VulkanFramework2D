#include "VulkanPhysicalDevice.h"
#include "Logs.h"

#include "VulkanInstance.h"

VulkanPhysicalDevice::VulkanPhysicalDevice(){
    mRequiredExtensions = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName
    };

}

VulkanPhysicalDevice::~VulkanPhysicalDevice(){

}

bool VulkanPhysicalDevice::SetupPhysicalDevice(const VulkanInstance& instance){

    if(!GetDeviceScores(instance)) { return false; }

    SelectBestDevice();

    return true;
}

bool VulkanPhysicalDevice::GetDeviceScores(const VulkanInstance& instance){

    vkEnumeratePhysicalDevices(instance.GetInstance(), &mPhyiscalDeviceCount, nullptr);

    if(mPhyiscalDeviceCount == 0){

        Logs::PrintError("No available Devices");
        return false;
    }

    std::vector<VkPhysicalDevice> devices;
    devices.resize(mPhyiscalDeviceCount);
    vkEnumeratePhysicalDevices(instance.GetInstance(), &mPhyiscalDeviceCount, devices.data());

    for(size_t index = 0; index < mPhyiscalDeviceCount; ++index)
    {
        size_t graphicsIndex = VulkanStructs::NO_FAMILY_INDEX;
        size_t presentIndex = VulkanStructs::NO_FAMILY_INDEX;
        if(!CheckQueueSupport(devices[index], graphicsIndex, presentIndex)) { continue; }

        if(!CheckExtensionSupport(devices[index])) { continue; }

        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(devices[index], &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures{};
        vkGetPhysicalDeviceFeatures(devices[index], &deviceFeatures);

        if(!CheckSuitable(devices[index], deviceProperties)){
            Logs::PrintError("Device Not Suitable");
            continue;
        }

        uint32_t score = ScoreDevice(deviceProperties, deviceFeatures);

        Logs::Print("Device : " + std::string(deviceProperties.deviceName) + " Scored: " + std::to_string(score));

        mAvailableDevices.emplace_back(devices[index], score, deviceProperties.deviceName, true, graphicsIndex, presentIndex);
    }

    return true;
}

bool VulkanPhysicalDevice::CheckSuitable(const VkPhysicalDevice& device, const VkPhysicalDeviceProperties& properties){

    bool propertyCheck = CheckDeviceProperties(properties);

    bool featureCheck = CheckDeviceFeatures(device);

    return propertyCheck && featureCheck;
}

bool VulkanPhysicalDevice::CheckDeviceProperties(const VkPhysicalDeviceProperties& properties){

    bool suitable = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;

    if(!suitable) { Logs::PrintError("No Suitable Device Type"); }

    return suitable;
}

bool VulkanPhysicalDevice::CheckDeviceFeatures(const VkPhysicalDevice& device){

    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extDynState{
        .sType =  VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
        .pNext = nullptr
    };

    VkPhysicalDeviceVulkan13Features vk13Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &extDynState
    };

    VkPhysicalDeviceFeatures2 features2{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &vk13Features
    };

    vkGetPhysicalDeviceFeatures2(device, &features2);

    bool requiredFeatures = vk13Features.dynamicRendering && extDynState.extendedDynamicState;

    if(!requiredFeatures) { Logs::PrintError("Required Features not supported"); }

    bool coreFeatures = features2.features.samplerAnisotropy;

    if(!coreFeatures) { Logs::PrintError("Core Features not supported"); }

    return requiredFeatures && coreFeatures;
}

uint32_t VulkanPhysicalDevice::ScoreDevice(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features){

    uint32_t score = 0;

    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            score += 2000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            score += 1000;
            break;
        default:
            score += 0;
            break;
    }

    if(properties.apiVersion >= VK_API_VERSION_1_3) { score += 300; }

    if (features.robustBufferAccess) { score+= 130; }
    if (features.wideLines) { score+= 100; }
    if (features.sampleRateShading) { score+= 70; }

    if (features.independentBlend) { score+= 60; }
    if (features.wideLines) { score+= 60; }
    if (features.depthClamp) { score+= 40; }
    if (features.fillModeNonSolid) { score+= 40; }

    if (features.occlusionQueryPrecise) { score+= 35; }
    if (features.depthBiasClamp) { score+= 33; }
    if (features.dualSrcBlend) { score+= 32; }

    return score;
}

void VulkanPhysicalDevice::SelectBestDevice(){

    if(mAvailableDevices.size() < 2)
    {
        mSelectedDevice = mAvailableDevices[0];

        Logs::Print("Selected Device: " + mSelectedDevice.deviceName);
        return;
    }

    for(int i = 0; i < mAvailableDevices.size() - 1; ++i){

        if(mAvailableDevices[i].score > mAvailableDevices[i + 1].score){
            mSelectedDevice = mAvailableDevices[i];
        }
        else{
            mSelectedDevice = mAvailableDevices[i + 1];
        }
    }

    Logs::Print("Selected Device: " + mSelectedDevice.deviceName);
}

bool VulkanPhysicalDevice::CheckQueueSupport(const VkPhysicalDevice& device, size_t& graphicsQueueIndex, size_t& presentQueueIndex){
    uint32_t familyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, queueFamilies.data());

    for(size_t i = 0; i < queueFamilies.size(); ++i)
    {
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && graphicsQueueIndex == VulkanStructs::NO_FAMILY_INDEX) {
            Logs::Print("Graphics Queue found at index " + std::to_string(i));
            graphicsQueueIndex = i;
            return true;
        }

        //VkBool32 presentSupport = VK_FALSE;
        //vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        //if(presentSupport && presentQueueIndex == VulkanStructs::NO_FAMILY_INDEX)
        //{
        //    Logs::Print("Present Queue found at index " + std::to_string(i));
        //    presentQueueIndex = i;
        //}
        //
        //if((presentQueueIndex != VulkanStructs::NO_FAMILY_INDEX) && (graphicsQueueIndex != VulkanStructs::NO_FAMILY_INDEX)) { return true;}
    }

    Logs::Print("Graphics Queue Not Supported");
    return false;
}

bool VulkanPhysicalDevice::CheckExtensionSupport(const VkPhysicalDevice& device){

    uint32_t availableExtensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, availableExtensions.data());

    int supportedExtensionsCount = 0;

    for(auto required : mRequiredExtensions)
    {
        for(auto available : availableExtensions)
        {
            if(std::strcmp(required, available.extensionName) == 0)
            {
                supportedExtensionsCount++;
                break;
            }
        }
    }

    bool allSupported = supportedExtensionsCount == mRequiredExtensions.size();

    if(!allSupported) { Logs::PrintError("Not All Extensions Supported"); }

    return allSupported;
}

void VulkanPhysicalDevice::SetQueues(){
    mQueueCreateInfos.clear();
    float queuePriority = 1.0f;
    uint32_t familyCount = 0;
    VkSharingMode shareMode = VK_SHARING_MODE_EXCLUSIVE;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = static_cast<uint32_t>(mSelectedDevice.graphicsQueueIndex),
      .queueCount = 1,
      .pQueuePriorities = &queuePriority,
    };
    mQueueCreateInfos.emplace_back(graphicsQueueCreateInfo);


    if(mSelectedDevice.graphicsQueueIndex != mSelectedDevice.presentQueueIndex) {
        VkDeviceQueueCreateInfo presentQueueCreateInfo = graphicsQueueCreateInfo;
        presentQueueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(mSelectedDevice.presentQueueIndex);
        mQueueCreateInfos.emplace_back(presentQueueCreateInfo);
        shareMode = VK_SHARING_MODE_CONCURRENT;
    }

    mSelectedDevice.shareMode = shareMode;
}
