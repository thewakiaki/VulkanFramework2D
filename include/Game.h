#pragma once

class GameWindow;
class VulkanInstance;
class WindowSurface;
class VulkanPhysicalDevice;
class VulkanLogicalDevice;
class VulkanSwapchain;
class VulkanPipeline;


class Game{
public:
    Game();
    ~Game();

    bool Run();


private:
    bool Startup();
    bool Play();
    void Cleanup();

    bool InitVulkan();

    std::unique_ptr<GameWindow> mGameWindow;
    std::unique_ptr<VulkanInstance> mVulkanInstance;
    std::unique_ptr<WindowSurface> mWindowSurface;
    std::unique_ptr<VulkanPhysicalDevice> mPhysicalDevice;
    std::unique_ptr<VulkanLogicalDevice> mLogicalDevice;
    std::unique_ptr<VulkanSwapchain> mSwapchain;
    std::unique_ptr<VulkanPipeline> mPipeline;
};
