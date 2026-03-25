#include "VulkanSwapchain.h"
#include "Logs.h"

#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "WindowSurface.h"
#include "GameWindow.h"

VulkanSwapchain::VulkanSwapchain(VulkanLogicalDevice& lDevice, WindowSurface& surface, VulkanPhysicalDevice& pDevice, GameWindow& window) :
                                 mLogicalDevice(lDevice), mWindowSurface(surface), mPhysicalDevice(pDevice), mWindow(window){

}

VulkanSwapchain::~VulkanSwapchain(){
    Cleanup();
}

bool VulkanSwapchain::SetupSwapchain(){

     Logs::Print("--------------------");
     Logs::Print("Setting up Swapchain");

     if(!SetSwapSurfaceFormat()) { return false; }

     if(!SetPresentMode()) { return false; }

     if(!SetSwapchainExtent()) { return false; }

     SetImageCount();

     VkSwapchainCreateInfoKHR createInfo = SetSwapchainCreateInfo();

     VkResult result = vkCreateSwapchainKHR(mLogicalDevice.GetLogicalDevice(), &createInfo, nullptr, &mSwapchain);

     if(result == VK_SUCCESS){
         Logs::Print("Swapchain Created");

         if(!RetriveSwapchainImages()) { return false; }

         if(!CreateSwapchainImageViews()) { return false; }

         Logs::Print("--------------------");

         return true;
     }

     Logs::PrintError("Failed to Create Swapchain", result);

     Logs::Print("--------------------");
     return false;
}

bool VulkanSwapchain::SetSwapSurfaceFormat(){

    if(mWindowSurface.GetFormats().empty())
    {
        Logs::PrintError("Failed to set Format: No Formats available");
        return false;
    }

    VkFormat preferredFormat = VK_FORMAT_R8G8B8A8_SRGB;
    VkColorSpaceKHR preferredColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    for(VkSurfaceFormatKHR format : mWindowSurface.GetFormats())
    {
        if((format.format == preferredFormat) && (format.colorSpace == preferredColorSpace))
        {

            Logs::Print("Preferred Format: VK_FORMAT_R8G8B8A8_SRGB found");
            Logs::Print("Preferred ColorSpace: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR found");
            mSwapSurfaceFormat = format;
            Logs::Print("Swap Surface format Set");
            return true;
        }
    }

    Logs::Print("Failed to find preferred format defaulting to first available format");

    if(mWindowSurface.GetFormats()[0].format == VK_FORMAT_UNDEFINED){
        Logs::Print("Surface format is VK_FORMAT_UNDEFINED; using preferred format");
        mSwapSurfaceFormat.format = preferredFormat;
        mSwapSurfaceFormat.colorSpace = preferredColorSpace;
    }
    else{
        mSwapSurfaceFormat = mWindowSurface.GetFormats()[0];
    }

    Logs::Print("Swap Surface format Set");

    return true;
}

bool VulkanSwapchain::SetPresentMode(){

    if(mWindowSurface.GetPresentModes().empty())
    {
        Logs::PrintError("No Present Modes Available");
        return false;
    }

    VkPresentModeKHR preferredMode = VK_PRESENT_MODE_MAILBOX_KHR;

    bool fifoModeFound = false;

    for(VkPresentModeKHR mode : mWindowSurface.GetPresentModes())
    {
        if(mode == VK_PRESENT_MODE_FIFO_KHR) { fifoModeFound = true; }

        if(mode == preferredMode)
        {
            Logs::Print("Preferred Present Mode: VK_PRESENT_MODE_MAILBOX_KHR found");
            mPresentMode = mode;
            Logs::Print("Present Mode Set");
            return true;
        }
    }

    if(!fifoModeFound){
        Logs::PrintError("VK_PRESENT_MODE_FIFO_KHR not found exiting application");
        return false;
    }

    Logs::Print("Preferred mode not found defaulting to VK_PRESENT_MODE_FIFO_KHR");
    mPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    Logs::Print("Present Mode Set");

    return true;
}

bool VulkanSwapchain::SetSwapchainExtent(){

    if(mWindowSurface.GetCapabilities().currentExtent.width != std::numeric_limits<uint32_t>::max()){

        mSwapchainExtent = mWindowSurface.GetCapabilities().currentExtent;

        if(!IsExtentValid(mSwapchainExtent)) { return false; }
    }
    else{

        int width, height;
        glfwGetFramebufferSize(const_cast<GLFWwindow*>(mWindow.GetGameWindow()), &width, &height);

        if(width == 0 || height == 0){
            Logs::PrintError("Invalid GLFW Framebuffer Size");
            return false;
        }

        mSwapchainExtent.width = std::clamp<uint32_t>(width, mWindowSurface.GetCapabilities().minImageExtent.width,
                                                    mWindowSurface.GetCapabilities().maxImageExtent.width);
        mSwapchainExtent.height = std::clamp<uint32_t>(height, mWindowSurface.GetCapabilities().minImageExtent.height,
                                                    mWindowSurface.GetCapabilities().maxImageExtent.height);
    }

    Logs::Print("Swapchain Extent Set");

    return true;
}

bool VulkanSwapchain::IsExtentValid(VkExtent2D& current){

    uint32_t maxWidth = mWindowSurface.GetCapabilities().maxImageExtent.width;
    uint32_t minWidth = mWindowSurface.GetCapabilities().minImageExtent.width;

    uint32_t maxHeight = mWindowSurface.GetCapabilities().maxImageExtent.height;
    uint32_t minHeight = mWindowSurface.GetCapabilities().minImageExtent.height;

    bool ExtentZeroValues = current.width == 0 || current.height == 0;

    bool ExtentOutsideWidth = current.width > maxWidth  || current.width < minWidth;
    bool ExtentOutsideHeight = current.height > maxHeight || current.height < minHeight;

    if(ExtentZeroValues || ExtentOutsideWidth || ExtentOutsideHeight)
    {
        Logs::PrintError("Invalid Extent");
        return false;
    }

    return true;
}

void VulkanSwapchain::SetImageCount(){

    uint32_t targetImageCount;
    uint32_t preferredMin = 3u;

    uint32_t minImageCount = mWindowSurface.GetCapabilities().minImageCount;
    uint32_t maxImageCount = mWindowSurface.GetCapabilities().maxImageCount;

    targetImageCount = minImageCount + 1;

    targetImageCount = std::max(preferredMin, targetImageCount);

    if(maxImageCount == 0)
    {
        mImageCount = targetImageCount;
        return;
    }

    mImageCount = std::clamp(targetImageCount, minImageCount, maxImageCount);\

    Logs::Print("Image Count Set");
}

VkSwapchainCreateInfoKHR VulkanSwapchain::SetSwapchainCreateInfo(){
    VkSwapchainCreateInfoKHR info{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = mWindowSurface.GetWindowSurface(),
        .minImageCount = mImageCount,
        .imageFormat = mSwapSurfaceFormat.format,
        .imageColorSpace = mSwapSurfaceFormat.colorSpace,
        .imageExtent = mSwapchainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = mPhysicalDevice.GetSelectedDevice().shareMode,
        .preTransform = mWindowSurface.GetCapabilities().currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = mPresentMode,
        .clipped = true
    };

    return info;
}

bool VulkanSwapchain::RetriveSwapchainImages(){

    uint32_t SwapchainImageCount = 0;
    VkResult result = vkGetSwapchainImagesKHR(mLogicalDevice.GetLogicalDevice(), mSwapchain, &SwapchainImageCount, nullptr);

    if(result != VK_SUCCESS && result != VK_INCOMPLETE){
        Logs::PrintError("Failed to get Swapchain Images: No Image Count", result);
        Logs::Print("--------------------");
        return false;
    }

    mSwapchainImages.resize(SwapchainImageCount);
    result = vkGetSwapchainImagesKHR(mLogicalDevice.GetLogicalDevice(), mSwapchain, &SwapchainImageCount, mSwapchainImages.data());

    if(result != VK_SUCCESS && result != VK_INCOMPLETE){
        Logs::PrintError("Failed to get image array", result);
        Logs::Print("--------------------");
        return false;
    }

    if(mSwapchainImages.empty()){
        Logs::PrintError("Failed to get Swapchain Images");
        Logs::Print("--------------------");

        return false;
    }

    Logs::Print("Retrieved Swapchain Images");
    return true;
}

bool VulkanSwapchain::CreateSwapchainImageViews(){
    mSwapchainImageViews.clear();

    VkImageViewCreateInfo imageViewInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .flags = 0,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = mSwapSurfaceFormat.format,
        .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}
    };

    mSwapchainImageViews.resize(mSwapchainImages.size());

    for(size_t image = 0; image < mSwapchainImages.size(); ++image)
    {
        imageViewInfo.image = mSwapchainImages[image];
        VkResult result = vkCreateImageView(mLogicalDevice.GetLogicalDevice(), &imageViewInfo, nullptr, &mSwapchainImageViews[image]);

        if(result != VK_SUCCESS)
        {
            Logs::PrintError("Failed to create image view", result);
            return false;
        }
    }

    Logs::Print("Created Swapchain Image Views");

    return true;
}

void VulkanSwapchain::Cleanup(){

    if(!mSwapchainImageViews.empty())
    {
        for(VkImageView& imageView : mSwapchainImageViews){
            vkDestroyImageView(mLogicalDevice.GetLogicalDevice(), imageView, nullptr);
            imageView = VK_NULL_HANDLE;
        }

        Logs::PrintComponentDestroyed("Image Views");
    }

    if(mSwapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(mLogicalDevice.GetLogicalDevice(), mSwapchain, nullptr);
        mSwapchain = VK_NULL_HANDLE;
        Logs::PrintComponentDestroyed("Swapchain");
    }


}
