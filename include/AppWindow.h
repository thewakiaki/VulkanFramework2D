#pragma once

class AppWindow{
public:
    AppWindow();
    ~AppWindow();

    AppWindow(const AppWindow&) = delete;
    AppWindow& operator=(const AppWindow&) = delete;

    bool SetupWindow();

private:
    void Cleanup();

    bool MakeWindow(int width = 800, int height=600, const char* title = "Vulkan");

    GLFWwindow* mWindow;
};
