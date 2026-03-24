#include "VulkanCommandPool.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"

VulkanCommandPool::VulkanCommandPool(const VulkanLogicalDevice& lDevice, const VulkanPhysicalDevice& pDevice)
                                        : mLogicalDevice(lDevice), mPhysicalDevice(pDevice){

}

VulkanCommandPool::~VulkanCommandPool(){
    Cleanup();

}

bool VulkanCommandPool::SetupCommandPool(){

    Logs::Print("--------------------");
    Logs::Print("Setting up Command Pool");

    VkCommandPoolCreateInfo poolInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = static_cast<uint32_t>(mPhysicalDevice.GetSelectedDevice().graphicsQueueIndex)
    };

    VkResult result = vkCreateCommandPool(mLogicalDevice.GetLogicalDevice(), &poolInfo, nullptr, &mCommandPool);

    if(result != VK_SUCCESS)
    {
        Logs::PrintError("Failed to Create Command Pool");
        return false;
    }

    Logs::Print("Command Pool Created");
    Logs::Print("--------------------");

    return true;
}

void VulkanCommandPool::Cleanup(){

    vkDeviceWaitIdle(mLogicalDevice.GetLogicalDevice());

    mCommandBuffer = VK_NULL_HANDLE;
    Logs::PrintComponentDestroyed("Command Buffer");


    if(mCommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(mLogicalDevice.GetLogicalDevice(), mCommandPool, nullptr);
        mCommandPool = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Command Pool");
    }

}
