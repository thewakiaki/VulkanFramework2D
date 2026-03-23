#pragma once

class GameWindow{
public:
    GameWindow();
    ~GameWindow();

    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;

    const GLFWwindow* GetGameWindow() const { return mWindow; }

    bool SetupWindow();

private:
    void Cleanup();

    bool MakeWindow(int width = 800, int height=600, const char* title = "Vulkan");

    GLFWwindow* mWindow;
};
