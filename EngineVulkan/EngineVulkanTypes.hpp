#pragma once 
#include <iostream>
#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace EngineVulkan
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	struct  SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
}