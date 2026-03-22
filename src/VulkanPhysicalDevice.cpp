#include "VulkanPhysicalDevice.h"
#include "Logs.h"

#include "VulkanInstance.h"

VulkanPhysicalDevice::VulkanPhysicalDevice(){

}

VulkanPhysicalDevice::~VulkanPhysicalDevice(){
    Cleanup();
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
        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(devices[index], &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures{};
        vkGetPhysicalDeviceFeatures(devices[index], &deviceFeatures);

        if(!CheckSuitable(deviceProperties, deviceFeatures)){
            Logs::PrintError("Device Not Suitable");
            continue;
        }

        uint32_t score = ScoreDevice(deviceProperties, deviceFeatures);

        Logs::Print("Device : " + std::string(deviceProperties.deviceName) + " Scored: " + std::to_string(score));

        mAvailableDevices.emplace_back(devices[index], score, deviceProperties.deviceName);
    }


    return true;
}

bool VulkanPhysicalDevice::CheckSuitable(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features){

    bool typeCheck = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;

    if( typeCheck && features.geometryShader)
    {
        return true;
    }

    return false;
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

    VulkanStructs::PhysicalDeviceDetails currentBest = mAvailableDevices[0];

    if(mAvailableDevices.size() < 2)
    {
        mSelectedDevice = currentBest.device;

        Logs::Print("Selected Device: " + currentBest.deviceName);
        return;
    }

    for(const VulkanStructs::PhysicalDeviceDetails& deviceDetails : mAvailableDevices){

        if(deviceDetails.score > currentBest.score){
            currentBest = deviceDetails;
        }
    }

    mSelectedDevice = currentBest.device;

    Logs::Print("Selected Device: " + currentBest.deviceName);
}

void VulkanPhysicalDevice::Cleanup(){


}
