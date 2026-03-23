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
    if(!SetupLayers()) { return false; }

    SetInstanceCreateInfo();

    VkResult result = vkCreateInstance(&mCreateInfo, nullptr, &mVulkanInstance);

    if(result == VK_SUCCESS)
    {
        Logs::Print("Vulkan Instance created");
        return true;
    }

    Logs::PrintError("Failed to create Vulkan Instance");

    return false;

}

void VulkanInstance::SetInstanceCreateInfo(){

    mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    mAppInfo.pNext = NULL;
    mAppInfo.pApplicationName = mGameName;
    mAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    mAppInfo.pEngineName = mEngineName;
    mAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    mAppInfo.apiVersion = VK_API_VERSION_1_3;

    mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    mCreateInfo.pNext = NULL;
    mCreateInfo.pApplicationInfo = &mAppInfo;
    mCreateInfo.enabledLayerCount = static_cast<uint32_t>(mValidatedLayers.size());
    mCreateInfo.ppEnabledLayerNames = mValidatedLayers.data();
    mCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mValidatedExtensions.size());
    mCreateInfo.ppEnabledExtensionNames = mValidatedExtensions.data();
}

bool VulkanInstance::SetupExtensions(){

    mRequiredExtensions.clear();

    mGLFWExtensions = glfwGetRequiredInstanceExtensions(&mGLFWExtensionCount);

    for(uint32_t extension = 0; extension < mGLFWExtensionCount; ++extension){
        mRequiredExtensions.emplace_back(mGLFWExtensions[extension]);
    }

    //Add required extensions here　/ ここで必須の拡張を設定してください
    //The base three are the required ones for any version of the framework / 下記の拡張は必要最低限の拡張です必ずこの３つをのこしてくたさい
    mRequiredExtensions.emplace_back("VK_EXT_debug_utils"); // Validation/Debugging / 検証・デバッグ用

    vkEnumerateInstanceExtensionProperties(nullptr, &mVkExtensionCount, nullptr);
    mAvailableExtensions.resize(mVkExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &mVkExtensionCount, mAvailableExtensions.data());

    mValidatedExtensions.clear();

    for(const auto& ext : mAvailableExtensions) {
        for(const auto& required : mRequiredExtensions) {
                if(std::strcmp(ext.extensionName, required) == 0) {
                    mValidatedExtensions.emplace_back(required);
                    break;
                }
        }
    }

    if(mValidatedExtensions.size() != mRequiredExtensions.size()) {

        Logs::PrintError("Not all required extensions supported");

        return false;
    }

    ViewValidatedExtensions();

    return true;
}

bool VulkanInstance::SetupLayers(){

    if(!enableValidationLayers) {
        mValidatedLayers.clear();
        return true;
    }

    mRequiredLayers = { "VK_LAYER_KHRONOS_validation" };

    vkEnumerateInstanceLayerProperties(&mVkLayerCount, nullptr);
    mAvailableLayers.resize(mVkLayerCount);
    vkEnumerateInstanceLayerProperties(&mVkLayerCount, mAvailableLayers.data());

    mValidatedLayers.clear();


    for(const auto& layer : mAvailableLayers) {
        bool isRequired = false;

        for(const auto& required : mRequiredLayers) {
                if(std::strcmp(layer.layerName, required) == 0) {
                    mValidatedLayers.emplace_back(required);
                    break;
                }
        }
    }

    if(mValidatedLayers.size() != mRequiredLayers.size()) {

        Logs::PrintError("Not all required layers supported");

        return false;
    }

    ViewValidatedLayers();

    return true;
}

void VulkanInstance::ViewValidatedExtensions(){

  Logs::Print("Added Extensions");
  Logs::Print("--------------------");

  for(auto ext : mValidatedExtensions)
  {
      if(ext)
      {
          Logs::Print(ext);
      }
  }
  Logs::Print("--------------------");
}

void VulkanInstance::ViewValidatedLayers(){

  Logs::Print("Added Layers");
  Logs::Print("--------------------");

  for(auto Layer : mValidatedLayers)
  {
      if(Layer)
      {
          Logs::Print(Layer);
      }
  }
  Logs::Print("--------------------");
}

void VulkanInstance::Cleanup(){

    if(mVulkanInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(mVulkanInstance, nullptr);
        mVulkanInstance = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Vulkan Instance");
    }
}
