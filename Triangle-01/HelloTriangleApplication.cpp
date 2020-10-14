#include "HelloTriangleApplication.hpp"
#include "QueueFamilyIndices.hpp"
#include "ExtensionUtils.hpp"

#include <iostream>
#include <set>

using namespace Triangle_01;

HelloTriangleApplication::HelloTriangleApplication() : 
    vkInstance_(VK_NULL_HANDLE), vkDebugMessenger_(VK_NULL_HANDLE), vkPhysicalDevice_(VK_NULL_HANDLE), window_(nullptr) {

}

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();

}

void HelloTriangleApplication::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
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

void HelloTriangleApplication::createSurface() {
    if (glfwCreateWindowSurface(vkInstance_, window_, nullptr, &vkSurface_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
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

    if (ExtensionUtils::CreateDebugUtilsMessengerEXT(vkInstance_, &createInfo, nullptr, &vkDebugMessenger_) != VK_SUCCESS) {
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

void HelloTriangleApplication::pickPhysicalDevice() {

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, vkSurface_)) {
            vkPhysicalDevice_ = device;
            break;
        }
    }

    if (vkPhysicalDevice_ == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }


}

bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, surface);
    return indices.isComplete() && checkDeviceExtensionSupport(device);
}

bool HelloTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void HelloTriangleApplication::mainLoop() {

}


void HelloTriangleApplication::createLogicalDevice() {
    QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(vkPhysicalDevice_,vkSurface_);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        auto queueCreateInfo = VkDeviceQueueCreateInfo();
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    auto deviceFeatures = VkPhysicalDeviceFeatures();

    auto createInfo = VkDeviceCreateInfo();
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VK_VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VK_VALIDATION_LAYERS.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(vkPhysicalDevice_, &createInfo, nullptr, &vkDevice_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(vkDevice_, indices.graphicsFamily.value(), 0, &vkGraphicsQueue_);
    vkGetDeviceQueue(vkDevice_, indices.presentFamily.value(), 0, &vkPresentQueue_);

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

void HelloTriangleApplication::cleanup() {
    vkDestroyDevice(vkDevice_, nullptr);

    if (enableValidationLayers) {
        ExtensionUtils::DestroyDebugUtilsMessengerEXT(vkInstance_, vkDebugMessenger_, nullptr);
    }

    vkDestroySurfaceKHR(vkInstance_, vkSurface_, nullptr);

    vkDestroyInstance(vkInstance_, nullptr);

    glfwDestroyWindow(window_);

    glfwTerminate();

}


