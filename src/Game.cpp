#include "Game.h"
#include "Logs.h"

#include "AppWindow.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

Game::Game(){
    mAppWindow = std::make_unique<AppWindow>();
    mVulkanInstance = std::make_unique<VulkanInstance>();
    mPhysicalDevice = std::make_unique<VulkanPhysicalDevice>();
}

Game::~Game(){

}

bool Game::Run(){

    if(!Startup()) { return false;}

    return false;
}

bool Game::Startup(){

    Logs::Print("Game Starting");

    if(!InitVulkan()) { return false; }

    return true;
}

bool Game::Play(){

    return true;
}

void Game::Cleanup(){

}

bool Game::InitVulkan(){

    if(!mVulkanInstance->SetupVulkanInstance()) { return false; }

    if(!mPhysicalDevice->SetupPhysicalDevice(*mVulkanInstance)) { return false; }

    Logs::Print("All Vulkan Components Setup");

    return true;
}
