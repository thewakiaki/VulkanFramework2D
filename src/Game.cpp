#include "Game.h"
#include "Logs.h"
#include "VulkanInstance.h"

Game::Game(){
    mVulkanInstance = std::make_unique<VulkanInstance>();
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

    Logs::Print("All Vulkan Components Setup");

    return true;
}
