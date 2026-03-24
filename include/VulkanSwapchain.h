#pragma once

class VulkanLogicalDevice;
class VulkanPhysicalDevice;
class WindowSurface;
class GameWindow;

class VulkanSwapchain{

public:
    VulkanSwapchain(VulkanLogicalDevice& lDevice, WindowSurface& surface, VulkanPhysicalDevice& pDevice, GameWindow& window);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

    const VkSwapchainKHR& GetSwapChain() const { return mSwapchain; }
    const VkExtent2D& GetSwapchainExtent() const { return mSwapchainExtent; }
    const VkSurfaceFormatKHR& GetSwapchainFormat() const { return mSwapSurfaceFormat; }
    const std::vector<VkImage>& GetSwapchainImages() const { return mSwapchainImages; }
    const std::vector<VkImageView>& GetSwapchainImageViews() const { return mSwapchainImageViews; }

    bool SetupSwapchain();

private:
    void Cleanup();

    bool SetSwapSurfaceFormat();
    bool SetPresentMode();
    bool SetSwapchainExtent();
    bool IsExtentValid(VkExtent2D& extent);
    bool RetriveSwapchainImages();
    bool CreateSwapchainImageViews();

    void SetImageCount();
    VkSwapchainCreateInfoKHR SetSwapchainCreateInfo();


    VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
    std::vector<VkImage> mSwapchainImages;
    std::vector<VkImageView> mSwapchainImageViews = { VK_NULL_HANDLE };

    VkExtent2D mSwapchainExtent = {0, 0};

    VkSurfaceFormatKHR mSwapSurfaceFormat;
    VkPresentModeKHR mPresentMode;

    uint32_t mImageCount = 0;

    const VulkanLogicalDevice& mLogicalDevice;
    const VulkanPhysicalDevice& mPhysicalDevice;
    const WindowSurface& mWindowSurface;
    const GameWindow& mWindow;
};
