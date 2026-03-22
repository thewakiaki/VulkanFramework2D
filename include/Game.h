#pragma once

class AppWindow;
class VulkanInstance;
class VulkanPhysicalDevice;


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

    std::unique_ptr<AppWindow> mAppWindow;
    std::unique_ptr<VulkanInstance> mVulkanInstance;
    std::unique_ptr<VulkanPhysicalDevice> mPhysicalDevice;
};
