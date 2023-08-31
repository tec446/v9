#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>

#include "SwapChain.h"

class PhysicalDevice
{
public:
	struct QueueFamilyIndices 
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	}; // struct QueueFamilyIndices

	const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	VkSampleCountFlagBits m_msaaSamples    = VK_SAMPLE_COUNT_1_BIT;
	VkPhysicalDevice      m_physicalDevice = VK_NULL_HANDLE;

	void pickPhysicalDevice(Instance& instance, SwapChain& swapChain, Surface& surface);
	bool isDeviceSuitable(VkPhysicalDevice& device, SwapChain& swapChain, Surface& surface);
	auto findQueueFamilies(VkPhysicalDevice& device, Surface& surface) -> PhysicalDevice::QueueFamilyIndices;
	bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
	auto getMaxUsableSampleCount() -> VkSampleCountFlagBits;

	// Allows us to use the class like a pointer
	auto operator*() -> VkPhysicalDevice& { return m_physicalDevice; }

}; // class PhysicalDevice