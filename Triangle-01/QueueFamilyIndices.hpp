#pragma once

#include <vulkan/vulkan.hpp>
#include <optional>

namespace Triangle_01 {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		bool isComplete(); 

	};

}
