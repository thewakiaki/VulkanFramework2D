#pragma once

class VulkanInstance;
class VulkanPhysicalDevice;
struct GameWindow;

class WindowSurface{

public:
    WindowSurface(VulkanInstance& instance, GameWindow& window);
    ~WindowSurface();

    WindowSurface(const WindowSurface&) = delete;
    WindowSurface& operator=(const WindowSurface&) = delete;

    bool SetupWindowSurface();
    void SetPhysicalDeviceSurfaceDetails(const VulkanPhysicalDevice& pDevice);

    const VkSurfaceKHR& GetWindowSurface() const { return mSurface; }
    const VkSurfaceCapabilitiesKHR& GetCapabilities() const { return mCapabilities; }
    const std::vector<VkSurfaceFormatKHR>& GetFormats() const { return mFormats; }
    const std::vector<VkPresentModeKHR>& GetPresentModes() const { return mAvailablePresentModes; }

private:
    void Cleanup();

    VkSurfaceKHR mSurface = VK_NULL_HANDLE;

    VkSurfaceCapabilitiesKHR mCapabilities;

    std::vector<VkSurfaceFormatKHR> mFormats;

    std::vector<VkPresentModeKHR> mAvailablePresentModes;

    const VulkanInstance& mInstance;
    const GameWindow& mWindow;

};
