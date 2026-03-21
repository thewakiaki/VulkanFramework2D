#include "VulkanInstance.h"
#include "Logs.h"

VulkanInstance::VulkanInstance(){

}

VulkanInstance::~VulkanInstance(){
    Cleanup();

}

bool VulkanInstance::SetupVulkanInstance(){

    Logs::Print("Setting Up Vulkan Instance");

    if(!SetupExtensions()) { return false; }

    try {
        mVulkanInstance = vk::createInstance(SetInstanceCreateInfo());
        Logs::Print("Sucessfully Created Vulkan Instance");

        return true;

    } catch (const vk::SystemError& error) {
        Logs::PrintError("Failed to create Vulkan Instance", error);

        return false;
    }

}

vk::InstanceCreateInfo VulkanInstance::SetInstanceCreateInfo(){
    vk::ApplicationInfo appInfo{
        .pApplicationName = mGameName,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = mEngineName,
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(mRequiredExtensions.size()),
        .ppEnabledExtensionNames = mRequiredExtensions.data()
    };


    return createInfo;
}

bool VulkanInstance::SetupExtensions(){

    //Add required extensions here　/ ここで必須の拡張を設定してください
    //The base three are the required ones for any version of the framework / 下記の拡張は必要最低限の拡張です必ずこの３つをのこしてくたさい
    mRequiredExtensions = { "VK_EXT_debug_utils", // Validation/Debugging / 検証・デバッグ用
                            "VK_KHR_xcb_surface",
                            "VK_KHR_surface" };

    vkEnumerateInstanceExtensionProperties(nullptr, &mVkExtensionCount, nullptr);
    mAvailableExtensions.resize(mVkExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &mVkExtensionCount, mAvailableExtensions.data());

    mExtensionFlags.clear();
    mExtensionFlags.reserve(mAvailableExtensions.size());

    for(const auto& ext : mAvailableExtensions) {
        bool isRequired = false;

        for(const auto& required : mRequiredExtensions) {
                if(std::strcmp(ext.extensionName, required) == 0) {
                    isRequired = true;
                    break;
                }
        }

         mExtensionFlags.emplace_back(VulkanStructs::ExtensionFlags(ext, isRequired));
    }

    ViewFlaggedExtensions();

    return true;
}

void VulkanInstance::ViewFlaggedExtensions(){

  Logs::Print("Added Extensions");
  Logs::Print("--------------------");

  for(auto ext : mExtensionFlags)
  {
      if(ext.enabled)
      {
          Logs::Print(ext.extension.extensionName);
      }
  }
  Logs::Print("--------------------");
}

void VulkanInstance::Cleanup(){

    if(mVulkanInstance)
    {
        mVulkanInstance.destroy(nullptr);
        mVulkanInstance = vk::Instance{};
    }
}
