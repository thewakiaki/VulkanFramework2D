#pragma once

//Change to false to disable validation layers // デバッグ用のレイヤーをいらない場合falseに変えてください
constexpr bool enableValidationLayers = true;

class VulkanInstance{

public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

    [[nodiscard]] const VkInstance& GetInstance() const { return mVulkanInstance; }

    bool SetupVulkanInstance();

private:
    void Cleanup();

    bool SetupExtensions();

    bool SetupLayers();

    void SetInstanceCreateInfo();

    void ViewValidatedExtensions();
    void ViewValidatedLayers();

    VkInstance mVulkanInstance = VK_NULL_HANDLE;
    VkInstanceCreateInfo mCreateInfo{};
    VkApplicationInfo mAppInfo{};

    const char* mGameName = "Game Name";
    const char* mEngineName = "Engine Name";

    uint32_t mVkExtensionCount = 0;
    std::vector<const char*> mValidatedExtensions;
    std::vector<VkExtensionProperties> mAvailableExtensions;
    std::vector<const char*> mRequiredExtensions;

    uint32_t mVkLayerCount = 0;
    std::vector<const char*> mValidatedLayers;
    std::vector<VkLayerProperties> mAvailableLayers;
    std::vector<const char*> mRequiredLayers;
};
