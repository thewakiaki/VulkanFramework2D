#pragma once

namespace VulkanStructs {

    struct PhysicalDeviceDetails{

      VkPhysicalDevice device;
      std::string deviceName;
      uint32_t score;


      PhysicalDeviceDetails(VkPhysicalDevice pDevice = VK_NULL_HANDLE, uint32_t scr = 0, std::string name = "") : device(pDevice), score(scr), deviceName(name) {}
    };
}
