//
// Created by Sebastian Snoer on 15/07/2024.
//

#include "VKContext.h"

namespace Kraken {
    VKContext::VKContext(const ApplicationInfo& appInfo, GLFWwindow *windowHandle) : m_WindowHandle(windowHandle), m_AppInfo(appInfo) {
    }

    void VKContext::Init() {
        KR_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);

        PFN_vkCreateInstance pfnCreateInstance = (PFN_vkCreateInstance) glfwGetInstanceProcAddress(NULL, "vkCreateInstance");

        // Create Vulkan Instance
        VkApplicationInfo applicationInfo;
        applicationInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion			= VK_MAKE_VERSION( 1, 0, 2 );
        applicationInfo.applicationVersion	= VK_MAKE_VERSION( 0, 0, 1 );
        applicationInfo.engineVersion		= VK_MAKE_VERSION( 0, 0, 1 );
    }
} // Kraken
