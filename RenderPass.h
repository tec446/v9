#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "Device.h"
#include "SwapChain.h"

class RenderPass
{
public:
	VkRenderPass m_renderPass;

	void createRenderPass(
		Device& device,
		SwapChain& swapChain
	);

	// Allows us to use the class like a pointer
	auto operator*() -> VkRenderPass& { return m_renderPass; }

}; // class RenderPass