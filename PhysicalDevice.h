#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>

#include "Types.h"

class PhysicalDevice
{
public:
	const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	VkSampleCountFlagBits m_msaaSamples    = VK_SAMPLE_COUNT_1_BIT;
	VkPhysicalDevice      m_physicalDevice = VK_NULL_HANDLE;

	void pickPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface);
	bool isDeviceSuitable(VkPhysicalDevice& device, VkSurfaceKHR& surface);
	bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
	auto findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) -> QueueFamilyIndices;
	auto getMaxUsableSampleCount() -> VkSampleCountFlagBits;

	static auto querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) -> SwapChainSupportDetails;
	
	// Allows us to use the class like a pointer
	auto operator*() -> VkPhysicalDevice& { return m_physicalDevice; }

}; // class PhysicalDevice