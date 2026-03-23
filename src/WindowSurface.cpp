#include "WindowSurface.h"
#include "Logs.h"

#include "VulkanInstance.h"
#include "GameWindow.h"

WindowSurface::WindowSurface(VulkanInstance& instance, GameWindow& window) : mInstance(instance), mWindow(window){

}

WindowSurface::~WindowSurface(){

    Cleanup();

}

bool WindowSurface::SetupWindowSurface(){
    Logs::Print("Setting up surface");

    VkResult result = glfwCreateWindowSurface(mInstance.GetInstance(), const_cast<GLFWwindow*>(mWindow.GetGameWindow()), nullptr, &mSurface);

    if(result == VK_SUCCESS)
    {
        Logs::Print("Window Surface Created");
        return true;
    }

    Logs::PrintError("Failed to create Window Surface");

    return false;
}

void WindowSurface::Cleanup(){

    if(mSurface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(mInstance.GetInstance(), mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Surface");
    }

}
