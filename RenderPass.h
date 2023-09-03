#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "PhysicalDevice.h"

class RenderPass
{
public:
	VkRenderPass m_renderPass;

	void createRenderPass(PhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkFormat& swapChainImageFormat, VkFormat& swapChainDepthFormat, RenderPass& renderPass);

	// Allows us to use the class like a pointer
	auto operator*() -> VkRenderPass& { return m_renderPass; }

}; // class RenderPass