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
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        bool isDeviceSuitable(VkPhysicalDevice device);
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

        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        const std::vector<const char*> VK_VALIDATION_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
                const bool enableValidationLayers = false;
        #else
                const bool enableValidationLayers = true;
        #endif


    };

}

