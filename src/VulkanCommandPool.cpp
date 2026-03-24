#include "VulkanCommandPool.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanPipeline.h"

VulkanCommandPool::VulkanCommandPool(const VulkanLogicalDevice& lDevice, const VulkanPhysicalDevice& pDevice,
                                        const VulkanSwapchain& swapchain, const VulkanPipeline& pipeline)
                                        : mLogicalDevice(lDevice), mPhysicalDevice(pDevice), mSwapchain(swapchain), mPipeline(pipeline){

    mStartImageTransition = VulkanStructs::ImageTransition();

    mEndImageTransition = VulkanStructs::ImageTransition();
    mEndImageTransition.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    mEndImageTransition.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    mEndImageTransition.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    mEndImageTransition.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
    mEndImageTransition.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    mEndImageTransition.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
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

bool VulkanCommandPool::SetupCommandBuffer(){

    Logs::Print("--------------------");
    Logs::Print("Setting up Command Buffer");

    VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = mCommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkResult result = vkAllocateCommandBuffers(mLogicalDevice.GetLogicalDevice(), &allocInfo, &mCommandBuffer);

    if(result != VK_SUCCESS)
    {
        Logs::PrintError("Failed to allocate Command Buffer");
        return false;
    }

    Logs::Print("Command Buffer Allocated");
    Logs::Print("--------------------");

    return true;
}

bool VulkanCommandPool::RecordCommandBuffer(uint32_t imageIndex){

    Logs::Print("--------------------");
    Logs::Print("Recording Command Buffer");

    VkCommandBufferBeginInfo beginInfo = SetCommandBeginInfo();
    VkResult result = vkBeginCommandBuffer(mCommandBuffer, &beginInfo);

    if(result != VK_SUCCESS)
    {
        Logs::PrintError("Failed to begin recording Command Buffer");
        return false;
    }

    TransitionImageLayout(imageIndex, mStartImageTransition);

    VkRenderingInfo renderInfo = SetRenderingInfo(imageIndex);

    vkCmdBeginRendering(mCommandBuffer, &renderInfo);

    vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline.GetPipeline());

    UpdateViewport();
    UpdateScissor();

    vkCmdSetViewport(mCommandBuffer, 0, 1, &mViewport);
    vkCmdSetScissor(mCommandBuffer, 0, 1, &mScissor);

    vkCmdDraw(mCommandBuffer, 3, 1, 0, 0);

    vkCmdEndRendering(mCommandBuffer);

    TransitionImageLayout(imageIndex, mEndImageTransition);

    vkEndCommandBuffer(mCommandBuffer);

    Logs::Print("Finished Recording CommandBuffer");
    Logs::Print("--------------------");
    return true;
}

VkCommandBufferBeginInfo VulkanCommandPool::SetCommandBeginInfo(){

    VkCommandBufferBeginInfo info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
        .pInheritanceInfo = nullptr,
    };

    return info;
}

VkRenderingInfo VulkanCommandPool::SetRenderingInfo(uint32_t imageIndex){

    VkRenderingAttachmentInfo attachInfo = SetRenderAttachmentInfo(imageIndex);

    VkRenderingInfo info{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderArea = { .offset = { 0, 0 }, .extent = mSwapchain.GetSwapchainExtent() },
        .layerCount = 1,
        .viewMask = 0,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachInfo,
        .pDepthAttachment = nullptr,
        .pStencilAttachment = nullptr
    };

    return info;
}

VkRenderingAttachmentInfo VulkanCommandPool::SetRenderAttachmentInfo(uint32_t imageIndex){
    VkClearValue clearColor{
        .color{ .float32{0.1f, 0.1f, 0.1f, 1.0f} },
    };

    VkRenderingAttachmentInfo info{
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .pNext = nullptr,
        .imageView = mSwapchain.GetSwapchainImageViews()[imageIndex],
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .resolveMode = VK_RESOLVE_MODE_NONE,
        .resolveImageView = VK_NULL_HANDLE,
        .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = clearColor
    };

    return info;
}

void VulkanCommandPool::TransitionImageLayout(uint32_t imageIndex, VulkanStructs::ImageTransition& imageTransition){

    VkImageSubresourceRange subresourceRange{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };

    VkImageMemoryBarrier2 barrier{
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = imageTransition.srcStageMask,
        .srcAccessMask = imageTransition.srcAccessMask,
        .dstStageMask = imageTransition.dstStageMask,
        .dstAccessMask = imageTransition.dstAccessMask,
        .oldLayout = imageTransition.oldLayout,
        .newLayout = imageTransition.newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = mSwapchain.GetSwapchainImages()[imageIndex],
        .subresourceRange = subresourceRange
    };

    VkDependencyInfo dependencyInfo{
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .dependencyFlags = 0,
        .memoryBarrierCount = 0,
        .pMemoryBarriers = nullptr,
        .bufferMemoryBarrierCount = 0,
        .pBufferMemoryBarriers = nullptr,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &barrier
    };

    vkCmdPipelineBarrier2(mCommandBuffer, &dependencyInfo);
}

void VulkanCommandPool::UpdateViewport(){

    mViewport = VkViewport(0.0f, 0.0f, static_cast<float>(mSwapchain.GetSwapchainExtent().width),
                            static_cast<float>(mSwapchain.GetSwapchainExtent().height), 0.0f, 1.0f);
}

void VulkanCommandPool::UpdateScissor(){

    mScissor = VkRect2D(VkOffset2D(0, 0), mSwapchain.GetSwapchainExtent());
}

void VulkanCommandPool::Cleanup(){

    vkDeviceWaitIdle(mLogicalDevice.GetLogicalDevice());

    if(mCommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(mLogicalDevice.GetLogicalDevice(), mCommandPool, nullptr);
        mCommandPool = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Command Pool");
    }

}
