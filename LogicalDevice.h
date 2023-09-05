#pragma once

#include <vulkan/vulkan.h>

#include "Config.h"
#include "PhysicalDevice.h"

class LogicalDevice
{
public:
	VkDevice m_logicalDevice;
	VkQueue  m_graphicsQueue;
	VkQueue  m_presentQueue;

	void createLogicalDevice(PhysicalDevice& physicalDevice, const std::vector<const char*>& validationLayers, VkSurfaceKHR& surface);

	// Allows us to use the class like a pointer
	auto operator*() -> VkDevice& { return m_logicalDevice; }

}; // class LogicalDevice
