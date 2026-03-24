#include "VulkanShaderModule.h"

#include "FileUtility.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"

VulkanShaderModule::VulkanShaderModule(const VulkanLogicalDevice& device , ShaderType type, std::string fileName)
                                        : mLogicalDevice(device), mShaderType(type), mFilename(fileName){

}

VulkanShaderModule::~VulkanShaderModule(){
    Cleanup();
}

bool VulkanShaderModule::SetupShaderModule(const VulkanLogicalDevice& device){

    Logs::Print("Setting up Shader Module");

    if(!GetShaderCode(mFilename)) { return false; }

    size_t codeSize = mShaderCode.size() * sizeof(char);

    VkShaderModuleCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = codeSize,
        .pCode = reinterpret_cast<const uint32_t*>(mShaderCode.data()),
    };

    VkResult result = vkCreateShaderModule(device.GetLogicalDevice(), &createInfo, nullptr, &mShaderModule);

    if(result == VK_SUCCESS)
    {
        std::string type = "";

        mShaderType == ShaderType::VERTEX ? type = " VERTEX" : type = " FRAGMENT";

        Logs::Print("Created Shader Module for: " + FileUtility::GetFileName(mFilename) + type);
        return true;
    }

    Logs::PrintError("Failed to create shader module for: " + mFilename);

    return false;
}

bool VulkanShaderModule::GetShaderCode(const std::string& fileName){
    mShaderCode.clear();

    mShaderCode = FileUtility::ParseShaderFile(fileName);

    if(mShaderCode.empty()){
        Logs::PrintError("Failed to read shader data");
        return false;
    }

    return true;
}

void VulkanShaderModule::Cleanup(){

    if(mShaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(mLogicalDevice.GetLogicalDevice(), mShaderModule, nullptr);
        mShaderModule = VK_NULL_HANDLE;
    }

    Logs::PrintComponentDestroyed("Shader Module");
}
