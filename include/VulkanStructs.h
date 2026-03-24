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
}
