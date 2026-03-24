#include "Game.h"
#include "Logs.h"

#include "GameWindow.h"
#include "VulkanInstance.h"
#include "WindowSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanPipeline.h"
#include "VulkanCommandPool.h"

Game::Game(){
    mGameWindow = std::make_unique<GameWindow>();
    mVulkanInstance = std::make_unique<VulkanInstance>();
    mWindowSurface = std::make_unique<WindowSurface>(*mVulkanInstance, *mGameWindow);
    mPhysicalDevice = std::make_unique<VulkanPhysicalDevice>(*mWindowSurface);
    mLogicalDevice = std::make_unique<VulkanLogicalDevice>();
    mSwapchain = std::make_unique<VulkanSwapchain>(*mLogicalDevice, *mWindowSurface, *mPhysicalDevice, *mGameWindow);
    mPipeline = std::make_unique<VulkanPipeline>(*mLogicalDevice, *mSwapchain);
    mCommandPool = std::make_unique<VulkanCommandPool>(*mLogicalDevice, *mPhysicalDevice, *mSwapchain, *mPipeline);
}

Game::~Game(){
    Cleanup();
}

bool Game::Run(){

    if(!Startup()) { return false;}

    return false;
}

bool Game::Startup(){

    Logs::Print("Game Starting");

    if(!mGameWindow->SetupWindow()) { return false; }

    if(!InitVulkan()) { return false; }

    return true;
}

bool Game::Play(){

    return true;
}

void Game::Cleanup(){

    Logs::Print("--------------------");
    Logs::Print("Destroying Components");
    Logs::Print("--------------------");
    mCommandPool.reset();
    mPipeline.reset();
    mSwapchain.reset();
    mLogicalDevice.reset();
    mPhysicalDevice.reset();
    mWindowSurface.reset();
    mVulkanInstance.reset();
    mGameWindow.reset();
    Logs::Print("--------------------");

}

bool Game::InitVulkan(){

    if(!mVulkanInstance->SetupVulkanInstance()) { return false; }

    if(!mWindowSurface->SetupWindowSurface()) { return false; }

    if(!mPhysicalDevice->SetupPhysicalDevice(*mVulkanInstance)) { return false; }

    mWindowSurface->SetPhysicalDeviceSurfaceDetails(*mPhysicalDevice);

    if(!mLogicalDevice->SetupLogicalDevice(*mPhysicalDevice)) { return false; }

    if(!mSwapchain->SetupSwapchain()) { return false; }

    if(!mPipeline->SetupPipeline()){ return false;}

    if(!mCommandPool->SetupCommandPool()) { return false; }

    if(!mCommandPool->SetupCommandBuffer()) { return false; }

    Logs::Print("All Vulkan Components Setup");

    return true;
}
