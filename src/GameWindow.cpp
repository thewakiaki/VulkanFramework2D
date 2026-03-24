#include "GameWindow.h"
#include "Logs.h"

GameWindow::GameWindow(){
    mWindow = nullptr;
}

GameWindow::~GameWindow(){
    Cleanup();
}

bool GameWindow::SetupWindow(){
    glfwInit();

    Logs::Print("--------------------");
    Logs::Print("Setting up Window");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return MakeWindow();
}

bool GameWindow::MakeWindow(int width, int height, const char* title){

    mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if(!mWindow)
    {
        Logs::PrintError("Failed to create GLFW Window");
        return false;
    }

    Logs::Print("Window Created");
    Logs::Print("--------------------");

    return true;
}

void GameWindow::Cleanup(){
    glfwDestroyWindow(mWindow);

    Logs::PrintComponentDestroyed("Window");

    glfwTerminate();
}
