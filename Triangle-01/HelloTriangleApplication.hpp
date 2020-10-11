#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace Triangle_01 {

    class HelloTriangleApplication {

    public:
        void run();
    private:

        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();
        void createInstance();
        bool checkValidationLayerSupport();

        GLFWwindow* window_;
        VkInstance vkInstance_;
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

