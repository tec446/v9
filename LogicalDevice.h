#pragma once

#include <vulkan/vulkan.h>

#include "vulkanConfig.h"
#include "PhysicalDevice.h"
#include "Instance.h"
#include "Surface.h"

class LogicalDevice
{
public:
	VkDevice m_logicalDevice;
	VkQueue  m_graphicsQueue;
	VkQueue  m_presentQueue;

	void createLogicalDevice(PhysicalDevice& physicalDevice, Instance& instance, Surface& surface);

	// Allows us to use the class like a pointer
	auto operator*() -> VkDevice& { return m_logicalDevice; }

}; // class LogicalDevice

