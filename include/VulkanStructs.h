#pragma once

namespace VulkanStructs {
    constexpr size_t NO_FAMILY_INDEX = std::numeric_limits<size_t>::max();

    struct PhysicalDeviceDetails{

      VkPhysicalDevice device = VK_NULL_HANDLE;
      std::string deviceName = "";
      uint32_t score = 0;
      bool graphicsSupported = false;
      size_t graphicsQueueIndex = NO_FAMILY_INDEX;
      size_t presentQueueIndex = NO_FAMILY_INDEX;
      VkSharingMode shareMode = VK_SHARING_MODE_EXCLUSIVE;


      PhysicalDeviceDetails(VkPhysicalDevice pDevice = VK_NULL_HANDLE, uint32_t scr = 0, std::string name = "",
                            bool graphics = false, size_t gIndex = NO_FAMILY_INDEX, size_t pIndex = NO_FAMILY_INDEX) :
                            device(pDevice), score(scr), deviceName(name), graphicsSupported(graphics),
                            graphicsQueueIndex(gIndex), presentQueueIndex(pIndex) {}
    };

    struct ImageTransition{
        VkImageLayout oldLayout;
        VkImageLayout newLayout;
        VkAccessFlags2 srcAccessMask;
        VkAccessFlags2 dstAccessMask;
        VkPipelineStageFlags2 srcStageMask;
        VkPipelineStageFlags2 dstStageMask;

        ImageTransition(VkImageLayout oLayout = VK_IMAGE_LAYOUT_UNDEFINED, VkImageLayout nLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VkAccessFlags2 srcAcc = VK_ACCESS_2_NONE, VkAccessFlags2 dstAcc = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                        VkPipelineStageFlags2 srcStg = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
                        VkPipelineStageFlags2 dstStg = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT)
                        : oldLayout(oLayout), newLayout(nLayout), srcAccessMask(srcAcc), dstAccessMask(dstAcc), srcStageMask(srcStg), dstStageMask(dstStg) {}
    };
}
