#include "WindowSurface.h"
#include "Logs.h"

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "GameWindow.h"

WindowSurface::WindowSurface(VulkanInstance& instance, GameWindow& window) : mInstance(instance), mWindow(window){

}

WindowSurface::~WindowSurface(){

    Cleanup();

}

bool WindowSurface::SetupWindowSurface(){
    Logs::Print("--------------------");
    Logs::Print("Setting up surface");

    VkResult result = glfwCreateWindowSurface(mInstance.GetInstance(), const_cast<GLFWwindow*>(mWindow.GetGameWindow()), nullptr, &mSurface);

    if(result != VK_SUCCESS)
    {
        Logs::PrintError("Failed to create Window Surface", result);
        return false;
    }

    Logs::Print("Window Surface Created");
    Logs::Print("--------------------");
    return true;
}

void WindowSurface::SetPhysicalDeviceSurfaceDetails(const VulkanPhysicalDevice& pDevice){

    Logs::Print("--------------------");
    Logs::Print("Setting surface details");

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice.GetSelectedDevice().device, mSurface, &mCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice.GetSelectedDevice().device, mSurface, &formatCount, nullptr);
    mFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice.GetSelectedDevice().device, mSurface, &formatCount, mFormats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice.GetSelectedDevice().device, mSurface, &presentModeCount, nullptr);
    mAvailablePresentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice.GetSelectedDevice().device, mSurface, &presentModeCount, mAvailablePresentModes.data());

    Logs::Print("--------------------");
}

void WindowSurface::Cleanup(){

    if(mSurface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(mInstance.GetInstance(), mSurface, nullptr);
        mSurface = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Surface");
    }

}
