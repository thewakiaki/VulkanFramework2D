#include "Game.h"
#include "Logs.h"

#include "GameWindow.h"
#include "VulkanInstance.h"
#include "WindowSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"

Game::Game(){
    mGameWindow = std::make_unique<GameWindow>();
    mVulkanInstance = std::make_unique<VulkanInstance>();
    mWindowSurface = std::make_unique<WindowSurface>(*mVulkanInstance, *mGameWindow);
    mPhysicalDevice = std::make_unique<VulkanPhysicalDevice>(*mWindowSurface);
    mLogicalDevice = std::make_unique<VulkanLogicalDevice>();
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
    mLogicalDevice.reset();
    mPhysicalDevice.reset();
    mWindowSurface.reset();
    mVulkanInstance.reset();
    mGameWindow.reset();
}

bool Game::InitVulkan(){

    if(!mVulkanInstance->SetupVulkanInstance()) { return false; }

    if(!mWindowSurface->SetupWindowSurface()) { return false; }

    if(!mPhysicalDevice->SetupPhysicalDevice(*mVulkanInstance)) { return false; }

    if(!mLogicalDevice->SetupLogicalDevice(*mPhysicalDevice)) { return false; }

    Logs::Print("All Vulkan Components Setup");

    return true;
}
