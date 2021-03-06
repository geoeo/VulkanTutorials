#include "QueueFamilyIndices.hpp"

using namespace Triangle_01;

QueueFamilyIndices QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    auto indices = QueueFamilyIndices();
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;

        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            indices.graphicsFamily = i;

        if (presentSupport) {
            indices.presentFamily = i;
        }
        
        if (indices.isComplete())
            break;

        i++;
    }

    return indices;
}

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}