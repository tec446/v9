#pragma once

#include <vulkan/vulkan.h>
#include <glm/fwd.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <chrono>

#include "Buffer.h"
#include "LogicalDevice.h"
#include "GraphicsPipeline.h"
#include "CommandPool.h"

class UniformBuffers
{
public:
	std::vector<VkBuffer>		m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*>			m_uniformBuffersMapped;

	void createUniformBuffers(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, int maxFramesInFlight);
	void createVertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
	void createIndexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
	void vkUpdateUniformBuffer(SwapChain& swapChain, uint32_t currentImage);

	// Allows us to use UniformBuffers as a pointer
	auto operator*()       -> std::vector<VkBuffer>& { return m_uniformBuffers; }
	auto operator[](int i) -> VkBuffer&				 { return m_uniformBuffers[i]; }

}; // class UniformBuffers