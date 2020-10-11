#include "HelloTriangleApplication.hpp"

using namespace Triangle_01;

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();

}

void HelloTriangleApplication::initVulkan() {

}

void HelloTriangleApplication::mainLoop() {

}

void HelloTriangleApplication::cleanup() {

}

void HelloTriangleApplication::initWindow() {

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

}