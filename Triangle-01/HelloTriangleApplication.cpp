#include "HelloTriangleApplication.hpp"

#include <iostream>

using namespace Triangle_01;

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();

}

void HelloTriangleApplication::initVulkan() {
    createInstance();

}

void HelloTriangleApplication::createInstance() {

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo vkAppInfo = VkApplicationInfo();
    vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vkAppInfo.pApplicationName = "Hello Triangle";
    vkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    vkAppInfo.pEngineName = "No Engine";
    vkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    vkAppInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo vkCreateInfo = VkInstanceCreateInfo();
    vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkCreateInfo.pApplicationInfo = &vkAppInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vkCreateInfo.enabledExtensionCount = glfwExtensionCount;
    vkCreateInfo.ppEnabledExtensionNames = glfwExtensions;

    if (enableValidationLayers) {
        vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(VK_VALIDATION_LAYERS.size());
        vkCreateInfo.ppEnabledLayerNames = VK_VALIDATION_LAYERS.data();
    }
    else {
        vkCreateInfo.enabledLayerCount = 0;
    }


    uint32_t vkExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(vkExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, extensions.data());

    std::cout << "available extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << ' '; 

        for (auto i = 0; i < glfwExtensionCount; i++) {
            auto glfwExtension = glfwExtensions[i];
            if (std::string(extension.extensionName) == std::string(glfwExtension)) {
                std::cout << "- Supported in GLFW";
            }
        }

        std::cout << '\n';

    }


    VkResult result = vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance_);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

}

bool HelloTriangleApplication::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    auto allLayersFound = true;

    for (const char* layerName : VK_VALIDATION_LAYERS) {
        auto layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (std::string(layerName) == std::string(layerProperties.layerName)) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            allLayersFound = false;
            break;
        }
    
    }

    return allLayersFound;

}

void HelloTriangleApplication::mainLoop() {

}

void HelloTriangleApplication::cleanup() {

    vkDestroyInstance(vkInstance_, nullptr);

    glfwDestroyWindow(window_);

    glfwTerminate();

}

void HelloTriangleApplication::initWindow() {

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

}