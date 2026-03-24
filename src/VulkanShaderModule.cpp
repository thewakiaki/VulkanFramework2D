#include "VulkanShaderModule.h"

#include "FileUtility.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"

VulkanShaderModule::VulkanShaderModule(const VulkanLogicalDevice& device) : mLogicalDevice(device){

}

VulkanShaderModule::~VulkanShaderModule(){
    Cleanup();
}

bool VulkanShaderModule::SetupShaderModule(const VulkanLogicalDevice& device, const std::string& fileName, ShaderType type){

    Logs::Print("--------------------");
    Logs::Print("Setting up Shader Module");

    mShaderType = type;

    if(!GetShaderCode(fileName)) { return false; }

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
        Logs::Print("Created Shader Module");
        Logs::Print("--------------------");
        return true;
    }

    Logs::PrintError("Failed to create shader module");
    Logs::Print("--------------------");

    return false;
}

bool VulkanShaderModule::GetShaderCode(const std::string& fileName){
    mShaderCode.clear();

    mShaderCode = FileUtility::ParseFile(fileName);

    if(mShaderCode.empty()){
        Logs::PrintError("Failed to read shader data");
        return false;
    }

    return true;
}
