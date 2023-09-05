#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>

#include "Config.h"
#include "Types.h"

class Device
{
public:
	const std::vector<const char*> m_deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkSampleCountFlagBits m_msaaSamples   = VK_SAMPLE_COUNT_1_BIT;
	VkPhysicalDevice      m_physical	  = VK_NULL_HANDLE;
	VkDevice			  m_logical;
	VkQueue				  m_graphicsQueue;
	VkQueue				  m_presentQueue;

	void pickPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface); 
	void createLogicalDevice(const std::vector<const char*>& validationLayers, VkSurfaceKHR& surface);
	auto getMaxUsableSampleCount() -> VkSampleCountFlagBits;
	bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
	bool isDeviceSuitable(VkPhysicalDevice& device, VkSurfaceKHR& surface);

	static auto findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface) -> QueueFamilyIndices;
	static auto querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) -> SwapChainSupportDetails;

	// Allows us to use the class like a pointer
	auto operator*() -> VkDevice& { return m_logical; }

}; // class PhysicalDevice