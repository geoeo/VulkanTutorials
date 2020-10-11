#pragma once

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

        GLFWwindow* window_;
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;

    };

}

