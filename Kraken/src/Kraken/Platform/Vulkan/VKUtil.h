//
// Created by Sebastian Snoer on 15/07/2024.
//

#pragma once
#include "Kraken/Assets/Asset.h"

namespace Kraken {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool IsComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VKExt {
    public:
        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    };

    class VKUtil {
    public:
        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

        static bool CheckDeviceExtensionsSupport(VkPhysicalDevice device, const std::vector<const char *> & vector);

        static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR &surface);
        static VkSurfaceFormatKHR ChooseOptimalSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR ChooseOptimalSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        static VkExtent2D ChooseOptimalSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        static VkShaderModule CreateShaderModule(AssetSpecification &spec);

        static uint32_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
        static VkPhysicalDevice PickBestSuitableDevice(std::vector<VkPhysicalDevice> devices, VkSurfaceKHR surface);
    };
} // Kraken
