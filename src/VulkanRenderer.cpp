#include "VulkanRenderer.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanCommandPool.h"

VulkanRenderer::VulkanRenderer(const VulkanLogicalDevice& lDevice, const VulkanSwapchain& swapchain, VulkanCommandPool& cmdPool) :
                                mLogicalDevice(lDevice), mSwapchain(swapchain), mCommandPool(cmdPool) {

}

VulkanRenderer::~VulkanRenderer(){
    Cleanup();
}

bool VulkanRenderer::SetupRenderer(){

    Logs::Print("--------------------");
    Logs::Print("Setting Up Renderer");

    if(!SetupSyncObjects()) { return false; }

    Logs::Print("Renderer Setup");
    Logs::Print("--------------------");

    return true;
}

void VulkanRenderer::DrawFrame(){

    VkResult fenceWaitResult = vkWaitForFences(mLogicalDevice.GetLogicalDevice(), 1, &mDrawFence, VK_TRUE, UINT64_MAX);

    if(fenceWaitResult != VK_SUCCESS){
        Logs::PrintError("Failed to wait for fences", fenceWaitResult);
        return;
    }

    uint32_t imageIndex;
    VkResult imageAcquireResult = vkAcquireNextImageKHR(mLogicalDevice.GetLogicalDevice(), mSwapchain.GetSwapChain(), UINT64_MAX,
                                            mRenderStartSemaphore, VK_NULL_HANDLE, &imageIndex);

    if(imageAcquireResult != VK_SUCCESS)
    {
        Logs::PrintError("Failed To Acquire Swapchain Image", imageAcquireResult);
        return;
    }

    mCommandPool.RecordCommandBuffer(imageIndex);

    VkResult resetFenceResult = vkResetFences(mLogicalDevice.GetLogicalDevice(), 1, &mDrawFence);

    if(resetFenceResult != VK_SUCCESS){
        Logs::PrintError("Failed to reset fence", resetFenceResult);
        return;
    }

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo sumbitInfo = SetSubmitInfo(waitStages);

    VkResult submitResult = vkQueueSubmit(mLogicalDevice.GetGraphicsQueue(), 1, &sumbitInfo, mDrawFence);

    if(submitResult != VK_SUCCESS){
        Logs::PrintError("Queue Submit Failed", submitResult);
        return;
    }

    VkPresentInfoKHR presentInfo = SetPresentInfo(imageIndex);

    VkResult presentResult = vkQueuePresentKHR(mLogicalDevice.GetPresentQueue(), &presentInfo);

    if(presentResult != VK_SUCCESS){
        Logs::PrintError("Failed to Present", presentResult);
        return;
    }

}

bool VulkanRenderer::SetupSyncObjects(){

    if(!SetupSemaphores()) { return false; }

    if(!SetupFences()) { return false; }

    return true;
}

bool VulkanRenderer::SetupSemaphores(){

    VkSemaphoreCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    VkResult result = vkCreateSemaphore(mLogicalDevice.GetLogicalDevice(), &createInfo, nullptr, &mRenderStartSemaphore);

    if(result != VK_SUCCESS){

        Logs::PrintError("Failed to create Start Semaphore", result);
        return false;
    }

    Logs::Print("Start Semaphore Created");

    result = vkCreateSemaphore(mLogicalDevice.GetLogicalDevice(), &createInfo, nullptr, &mRenderFinishSemaphore);

    if(result != VK_SUCCESS){

        Logs::PrintError("Failed to create Finish Semaphore", result);
        return false;
    }

    Logs::Print("Finish Semaphore Created");

    return true;
}

bool VulkanRenderer::SetupFences(){

    VkFenceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    VkResult result = vkCreateFence(mLogicalDevice.GetLogicalDevice(), &createInfo, nullptr, &mDrawFence);

    if(result != VK_SUCCESS){

        Logs::PrintError("Failed to create Draw Fence", result);
        return false;
    }

    Logs::Print("Draw Fence Created");

    return true;
}

VkSubmitInfo VulkanRenderer::SetSubmitInfo(VkPipelineStageFlags* waitStages){

    VkSubmitInfo info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &mRenderStartSemaphore,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &mCommandPool.GetCommandBuffer(),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &mRenderFinishSemaphore,
    };

    return info;
}

VkPresentInfoKHR VulkanRenderer::SetPresentInfo(uint32_t imageIndex){

    VkPresentInfoKHR info{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &mRenderFinishSemaphore,
        .swapchainCount = 1,
        .pSwapchains = &mSwapchain.GetSwapChain(),
        .pImageIndices = &imageIndex,
        .pResults = nullptr
    };

    return info;
}

void VulkanRenderer::Cleanup(){

    vkDeviceWaitIdle(mLogicalDevice.GetLogicalDevice());

    if(mRenderStartSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(mLogicalDevice.GetLogicalDevice(), mRenderStartSemaphore, nullptr);
        mRenderStartSemaphore = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Start Semaphore");
    }

    if(mRenderFinishSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(mLogicalDevice.GetLogicalDevice(), mRenderFinishSemaphore, nullptr);
        mRenderFinishSemaphore = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Finish Semaphore");
    }

    if(mDrawFence != VK_NULL_HANDLE)
    {
        vkDestroyFence(mLogicalDevice.GetLogicalDevice(), mDrawFence, nullptr);
        mDrawFence = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Draw Fence");
    }
}
