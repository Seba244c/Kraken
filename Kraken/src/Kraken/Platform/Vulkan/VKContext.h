//
// Created by Sebastian Snoer on 15/07/2024.
//

#pragma once

#include <krpch.h>
#include "Kraken/Graphics/GraphicsContext.h"
#include "Kraken/Graphics/Shader.h"

namespace Kraken {
    class VKContext final : public GraphicsContext {
    public:
        static VKContext& GetInstance() { return *s_Instance; }
        explicit VKContext(const ApplicationInfo& appInfo, GLFWwindow* windowHandle);
        ~VKContext();

        void Init() override;

        [[nodiscard]] VkSurfaceKHR GetSurface() const { return surface; }
        [[nodiscard]] VkDevice GetDevice() const { return device; }
        [[nodiscard]] VkQueue GetGraphicsQueue() const { return graphicsQueue; }
        [[nodiscard]] VkQueue GetPresentQueue() const { return presentQueue; }
    private:
        static VKContext* s_Instance;
        GLFWwindow* m_WindowHandle;
        const ApplicationInfo m_AppInfo;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        VkShaderModule vertShader, fragShader;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;

        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;

        std::vector<VkExtensionProperties> m_SupportedExtensions;
    private:
        void QuerySupportedExtensions();
        bool IsExtensionSupported(const char *extension);

        static bool CheckValidationLayerSupport(const std::vector<const char*>& layers);
        static std::vector<const char*> GetRequiredExtensions();
        std::unordered_map<ShaderType, VkShaderModule> CreateShaderModules(AssetSpecification &specs);

    private:
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateCommandBuffer();
        void CreateSyncObjects();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    public:
        void WaitForFrame() override;

        void NewFrame() override;

        void ShowFrame() override;

        void EnsureIdle() override;
    };
} // Kraken
