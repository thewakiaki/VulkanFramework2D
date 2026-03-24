#include "VulkanPipeline.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"
#include "VulkanShaderModule.h"

VulkanPipeline::VulkanPipeline(const VulkanLogicalDevice& lDevice) : mLogicalDevice(lDevice){
    mShaderModules.clear();

}

VulkanPipeline::~VulkanPipeline(){
    mShaderModules.clear();

    Cleanup();
}

bool VulkanPipeline::SetupPipeline(){

    Logs::Print("Setting Up Graphics Pipeline");

    if(!SetShaderModules()) { return false; }

    SetShaderStageCreateInfo();

    //VkPipelineShaderStageCreateInfo vertStageInfo =

    return true;
}

bool VulkanPipeline::SetShaderModules(){

    std::filesystem::path shadersDir(PROJECT_ROOT);
    shadersDir /= "Assets";
    shadersDir /= "Shaders";

    Logs::Print("--------------------");
    for(const auto& entry : std::filesystem::directory_iterator(shadersDir))
    {
        if((entry.path().extension() != ".spv") || !entry.is_regular_file()) { continue; }
        mShaderModules.emplace_back(std::make_unique<VulkanShaderModule>(mLogicalDevice, ShaderType::VERTEX, entry.path().native()));
        mShaderModules.emplace_back(std::make_unique<VulkanShaderModule>(mLogicalDevice, ShaderType::FRAGMENT, entry.path().native()));
    }

    if(mShaderModules.empty()){
        Logs::PrintError("Failed to load shader modules");
        return false;
    }

    for(const std::unique_ptr<VulkanShaderModule>& module : mShaderModules)
    {
        module->SetupShaderModule(mLogicalDevice);
    }

    Logs::Print("--------------------");

    return true;

}

void VulkanPipeline::SetShaderStageCreateInfo(){

    for(const auto& module : mShaderModules){
        VkPipelineShaderStageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.flags = 0;

        createInfo.module = module->GetShaderModule();

        if(module->GetShaderType() == ShaderType::VERTEX){
            createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            createInfo.pName = "vertMain";
        }
        else{
            createInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            createInfo.pName = "fragMain";
        }

        mShaderStageCreateInfos.emplace_back(createInfo);
    }
}

void VulkanPipeline::Cleanup(){

    mShaderModules.clear();

    //if(mPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(mLogicalDevice.GetLogicalDevice(), mPipeline, nullptr);
        mPipeline = VK_NULL_HANDLE;
    }
    Logs::PrintComponentDestroyed("Pipeline");
}
