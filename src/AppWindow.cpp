#include "AppWindow.h"
#include "Logs.h"

AppWindow::AppWindow(){
    mWindow = nullptr;
}

AppWindow::~AppWindow(){
    Cleanup();
}

bool AppWindow::SetupWindow(){
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return MakeWindow();
}

bool AppWindow::MakeWindow(int width, int height, const char* title){

    mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if(!mWindow)
    {
        Logs::PrintError("Failed to create GLFW Window");
        return false;
    }

    return true;
}

void AppWindow::Cleanup(){
    glfwDestroyWindow(mWindow);

    glfwTerminate();
}
