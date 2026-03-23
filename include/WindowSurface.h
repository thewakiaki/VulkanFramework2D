#pragma once

class VulkanInstance;
struct GameWindow;

class WindowSurface{

public:
    WindowSurface(VulkanInstance& instance, GameWindow& window);
    ~WindowSurface();

    WindowSurface(const WindowSurface&) = delete;
    WindowSurface& operator=(const WindowSurface&) = delete;

    bool SetupWindowSurface();

    const VkSurfaceKHR& GetWindowSurface() const { return mSurface; }

private:
    void Cleanup();

    VkSurfaceKHR mSurface = VK_NULL_HANDLE;

    const VulkanInstance& mInstance;
    const GameWindow& mWindow;

};
