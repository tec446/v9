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
	const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkSampleCountFlagBits m_msaaSamples    = VK_SAMPLE_COUNT_1_BIT;
	VkPhysicalDevice      m_physicalDevice = VK_NULL_HANDLE;

	void pickPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface);
	auto getMaxUsableSampleCount() -> VkSampleCountFlagBits;
	bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
	bool isDeviceSuitable(VkPhysicalDevice& device, VkSurfaceKHR& surface);

	static auto findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) -> QueueFamilyIndices;
	static auto querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) -> SwapChainSupportDetails;

	// Allows us to use the class like a pointer
	auto operator*() -> VkPhysicalDevice& { return m_physicalDevice; }

}; // class PhysicalDevice