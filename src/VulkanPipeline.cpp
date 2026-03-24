#include "VulkanPipeline.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"
#include "VulkanShaderModule.h"
#include "VulkanSwapchain.h"

VulkanPipeline::VulkanPipeline(const VulkanLogicalDevice& lDevice, const VulkanSwapchain& swapchain)
                                : mLogicalDevice(lDevice), mSwapchain(swapchain){
    mShaderModules.clear();

    mDynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    mViewport = VkViewport{
        0.0f, 0.0f,
        static_cast<float>(mSwapchain.GetSwapchainExtent().width), static_cast<float>(mSwapchain.GetSwapchainExtent().height),
        0.0f, 1.0f
    };

    mScissor = { VkOffset2D{0,0}, mSwapchain.GetSwapchainExtent() };
}

VulkanPipeline::~VulkanPipeline(){
    mShaderModules.clear();

    Cleanup();
}

bool VulkanPipeline::SetupPipeline(){

    Logs::Print("--------------------");
    Logs::Print("Setting Up Graphics Pipeline");

    if(!SetShaderModules()) { return false; }

    SetShaderStageCreateInfo();

    VkPipelineDynamicStateCreateInfo dynamicInfo = SetDynamicStateCreateInfo();
    VkPipelineVertexInputStateCreateInfo vertInputInfo = SetVertexInputCreateInfo();
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = SetInputAssemblyCreateInfo();
    VkPipelineViewportStateCreateInfo viewportInfo = SetViewportCreateInfo();
    VkPipelineRasterizationStateCreateInfo rasterInfo = SetRasterCreateInfo();
    VkPipelineMultisampleStateCreateInfo multiSampleInfo = SetMultiSampleCreateInfo();
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = SetDepthStencilCreateInfo();
    VkPipelineColorBlendStateCreateInfo colorBlendInfo = SetColorBlendCreateInfo();
    VkPipelineLayoutCreateInfo layoutInfo = SetLayoutCreateInfo();
    VkPipelineRenderingCreateInfo renderInfo = SetRenderCreateInfo();

    VkResult result = vkCreatePipelineLayout(mLogicalDevice.GetLogicalDevice(), &layoutInfo, nullptr, &mPipelineLayout);

    if(result != VK_SUCCESS){

        Logs::PrintError("Failed to create pipeline layout");
        return false;
    }

    Logs::Print("Created Pipeline Layout");

    VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &renderInfo,
        .flags = 0,
        .stageCount = static_cast<uint32_t>(mShaderStageCreateInfos.size()),
        .pStages = mShaderStageCreateInfos.data(),
        .pVertexInputState = &vertInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pTessellationState = nullptr,
        .pViewportState = &viewportInfo,
        .pRasterizationState = &rasterInfo,
        .pMultisampleState = &multiSampleInfo,
        .pColorBlendState = &colorBlendInfo,
        .pDynamicState = &dynamicInfo,
        .layout = mPipelineLayout,
        .renderPass = nullptr,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1,
    };

    result = vkCreateGraphicsPipelines(mLogicalDevice.GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline);

    if(result != VK_SUCCESS){

        Logs::PrintError("Failed to create Graphics Pipeline");
        return false;
    }

    Logs::Print("Created Graphics Pipeline");
    Logs::Print("--------------------");

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

VkPipelineDynamicStateCreateInfo VulkanPipeline::SetDynamicStateCreateInfo(){

    VkPipelineDynamicStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = static_cast<uint32_t>(mDynamicStates.size()),
        .pDynamicStates = mDynamicStates.data(),
    };

    return info;
}

VkPipelineVertexInputStateCreateInfo VulkanPipeline::SetVertexInputCreateInfo(){

    VkPipelineVertexInputStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .flags = 0,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = {},
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = {},
    };

    return info;
}

VkPipelineInputAssemblyStateCreateInfo VulkanPipeline::SetInputAssemblyCreateInfo(){

    VkPipelineInputAssemblyStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    return info;
}

VkPipelineViewportStateCreateInfo VulkanPipeline::SetViewportCreateInfo(){

    VkPipelineViewportStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = &mViewport,
        .scissorCount = 1,
        .pScissors = &mScissor
    };

    return info;
}

VkPipelineRasterizationStateCreateInfo VulkanPipeline::SetRasterCreateInfo(){

    VkPipelineRasterizationStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace =VK_FRONT_FACE_CLOCKWISE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f
    };

    return info;
}

VkPipelineMultisampleStateCreateInfo VulkanPipeline::SetMultiSampleCreateInfo(){

    VkPipelineMultisampleStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    };

    return info;
}

VkPipelineDepthStencilStateCreateInfo VulkanPipeline::SetDepthStencilCreateInfo(){

    VkPipelineDepthStencilStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthTestEnable = VK_FALSE,
        .depthWriteEnable = VK_FALSE,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        .front = {},
        .back = {},
        .minDepthBounds = 0.0f,
        .maxDepthBounds = 1.0f
    };

    return info;
}

VkPipelineColorBlendStateCreateInfo VulkanPipeline::SetColorBlendCreateInfo(){

    VkPipelineColorBlendAttachmentState attachment{
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &attachment,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
    };

    return info;
}

VkPipelineLayoutCreateInfo VulkanPipeline::SetLayoutCreateInfo(){

    VkPipelineLayoutCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    return info;
}

VkPipelineRenderingCreateInfo VulkanPipeline::SetRenderCreateInfo(){

    VkPipelineRenderingCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .pNext = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &mSwapchain.GetSwapchainFormat().format,
        .depthAttachmentFormat = VK_FORMAT_UNDEFINED,
        .stencilAttachmentFormat = VK_FORMAT_UNDEFINED
    };

    return info;
}

void VulkanPipeline::Cleanup(){

    mShaderModules.clear();

    if(mPipelineLayout != VK_NULL_HANDLE){

        vkDestroyPipelineLayout(mLogicalDevice.GetLogicalDevice(), mPipelineLayout, nullptr);
        mPipelineLayout = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Pipeline Layout");
    }

    if(mPipeline != VK_NULL_HANDLE){

        vkDestroyPipeline(mLogicalDevice.GetLogicalDevice(), mPipeline, nullptr);
        mPipeline = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Pipeline");
    }

}
