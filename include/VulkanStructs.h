#pragma once

namespace VulkanStructs {

    struct ExtensionFlags{
        bool enabled = false;
        VkExtensionProperties extension;

        ExtensionFlags(VkExtensionProperties ext, bool flag) : extension(ext), enabled(flag) {}
    };
}
