#include "HelloTriangleApplication.hpp"
#include "ExtensionUtils.hpp"

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
    setupDebugMessenger();
}

void HelloTriangleApplication::createInstance() {

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    auto vkAppInfo = VkApplicationInfo();
    vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vkAppInfo.pApplicationName = "Hello Triangle";
    vkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    vkAppInfo.pEngineName = "No Engine";
    vkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    vkAppInfo.apiVersion = VK_API_VERSION_1_2;

    auto vkCreateInfo = VkInstanceCreateInfo();
    vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkCreateInfo.pApplicationInfo = &vkAppInfo;

    //uint32_t glfwExtensionCount = 0;
    //const char** glfwExtensions;
    //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    //vkCreateInfo.enabledExtensionCount = glfwExtensionCount;
    //vkCreateInfo.ppEnabledExtensionNames = glfwExtensions;

    auto requiredExtensions = getRequiredExtensions();
    vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    vkCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(VK_VALIDATION_LAYERS.size());
        vkCreateInfo.ppEnabledLayerNames = VK_VALIDATION_LAYERS.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        vkCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        vkCreateInfo.enabledLayerCount = 0;
        vkCreateInfo.pNext = nullptr;
    }


    uint32_t vkExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
    std::vector<VkExtensionProperties> avaliable_vulkan_extensions(vkExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, avaliable_vulkan_extensions.data());

    std::cout << "available extensions:\n";

    for (const auto& extension : avaliable_vulkan_extensions) {
        std::cout << '\t' << extension.extensionName << ' '; 

        for (auto i = 0; i < requiredExtensions.size(); i++) {
            auto glfwExtension = requiredExtensions[i];
            if (std::string(extension.extensionName) == std::string(glfwExtension)) {
                std::cout << "- Required";
            }
        }

        std::cout << '\n';

    }

    VkResult result = vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance_);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;

}

void HelloTriangleApplication::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    auto createInfo = VkDebugUtilsMessengerCreateInfoEXT();
    populateDebugMessengerCreateInfo(createInfo);

    if (ExtensionUtils::CreateDebugUtilsMessengerEXT(vkInstance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }


}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = VkDebugUtilsMessengerCreateInfoEXT();
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
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

    if (enableValidationLayers) {
        ExtensionUtils::DestroyDebugUtilsMessengerEXT(vkInstance_, debugMessenger_, nullptr);
    }

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

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}