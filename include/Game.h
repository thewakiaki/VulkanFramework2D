#pragma once

class VulkanInstance;

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

    std::unique_ptr<VulkanInstance> mVulkanInstance;
};
