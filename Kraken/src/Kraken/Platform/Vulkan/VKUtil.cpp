//
// Created by Sebastian Snoer on 15/07/2024.
//

#include "VKUtil.h"

#include "Kraken/Assets/Asset.h"
#include "Kraken/Core/Application.h"
#include "Kraken/Graphics/Shader.h"

namespace Kraken {
    const std::vector<const char*> s_RequiredDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkResult VKExt::CreateDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT*pDebugMessenger) {

        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VKExt::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator) {

        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }



    QueueFamilyIndices VKUtil::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
        KR_PROFILE_FUNCTION();

        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for(int i = 0; i < queueFamilies.size(); i++) {
            const auto& queueFamily = queueFamilies[i];

            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if(presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.IsComplete()) break;
        }

        return indices;
    }

    SwapChainSupportDetails VKUtil::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface) {
        SwapChainSupportDetails details;

        // Basic Surface Capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        // Supported Surface Formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        // Supported presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    bool VKUtil::CheckDeviceExtensionsSupport(VkPhysicalDevice device, const std::vector<const char *> &requirements) {
        KR_PROFILE_FUNCTION();

        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(requirements.begin(), requirements.end());
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    VkSurfaceFormatKHR VKUtil::ChooseOptimalSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        KRC_WARN("VK: SwapSurfaceFormat: SRGB RGBA8, was not available");

        return availableFormats[0];
    }

    VkPresentModeKHR VKUtil::ChooseOptimalSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VKUtil::ChooseOptimalSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        int width = Application::GetInstance().GetWindow().GetFBWidth();
        int height = Application::GetInstance().GetWindow().GetFBHeight();

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }

    VkShaderModule VKUtil::CreateShaderModule(AssetSpecification& spec) {
        return nullptr;
    }

    uint32_t VKUtil::RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface) {
        KR_PROFILE_FUNCTION();

        VkPhysicalDeviceProperties deviceProps;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProps);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        uint32_t score = deviceProps.limits.maxImageDimension2D;

        if(!FindQueueFamilies(device, surface).IsComplete()) score *= 0;

        auto const extensionsSupported = CheckDeviceExtensionsSupport(device, s_RequiredDeviceExtensions);
        bool swapChainAdequate = false;
        if(extensionsSupported) {
            const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        if(!extensionsSupported || !swapChainAdequate) score *= 0;
        if(deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score *= 10;

        KRC_TRACE("Found device: {} (Score {})", deviceProps.deviceName, score);
        return score;
    }

    VkPhysicalDevice VKUtil::PickBestSuitableDevice(std::vector<VkPhysicalDevice> devices, VkSurfaceKHR surface) {
        KR_PROFILE_FUNCTION();

        std::multimap<uint32_t, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            uint32_t score = RateDeviceSuitability(device, surface);
            if(score == 0) continue;

            candidates.insert(std::make_pair(score, device));
        }

        // Return null if no candiate was suitable at all
        if(candidates.empty()) return VK_NULL_HANDLE;
        return candidates.rbegin()->second;
    }
} // Kraken