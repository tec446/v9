#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "PhysicalDevice.h"
#include "SwapChain.h"

class RenderPass
{
public:
	VkRenderPass m_renderPass;

	void createRenderPass(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain, RenderPass& renderPass);


	// Allows us to use the class like a pointer
	auto operator*() -> VkRenderPass& { return m_renderPass; }

}; // class RenderPass