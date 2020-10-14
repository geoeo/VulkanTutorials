#pragma once

#include <vulkan/vulkan.hpp>
#include <optional>

namespace Triangle_01 {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

		bool isComplete(); 

	};

}
