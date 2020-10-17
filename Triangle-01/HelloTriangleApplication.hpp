#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>



namespace Triangle_01 {

    class HelloTriangleApplication {

    public:
        void run();
        explicit HelloTriangleApplication();
    private:

        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();
        void createInstance();
        bool checkValidationLayerSupport();
        void setupDebugMessenger();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSurface();
        void createSwapChain();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        void createImageViews();
        std::vector<const char*> getRequiredExtensions();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData); 

        GLFWwindow* window_;
        VkInstance vkInstance_;
        VkDebugUtilsMessengerEXT vkDebugMessenger_;
        VkPhysicalDevice vkPhysicalDevice_;
        VkDevice vkDevice_;
        VkQueue vkGraphicsQueue_;
        VkQueue vkPresentQueue_;
        VkSurfaceKHR vkSurface_;
        VkSwapchainKHR vkSwapChain_;

        std::vector<VkImage> vkSwapChainImages_;
        std::vector<VkImageView> vkSwapChainImageViews_;
        VkFormat vkSwapChainImageFormat_;
        VkExtent2D vkSwapChainExtent_;

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char*> VK_VALIDATION_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> VK_DEVICE_EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        #ifdef NDEBUG
                const bool enableValidationLayers = false;
        #else
                const bool enableValidationLayers = true;
        #endif


    };

}

