#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "DescriptorSet.h"
#include "GraphicsPipeline.h"

class CommandPool
{
public:
	std::vector<VkSemaphore>	 m_imageAvailableSemaphores;
	std::vector<VkSemaphore>	 m_renderFinishedSemaphores;
	std::vector<VkFence>		 m_inFlightFences;
	uint32_t					 m_currentFrame = 0;
	VkBuffer					 m_vertexBuffer;
	VkDeviceMemory				 m_vertexBufferMemory;
	VkBuffer					 m_indexBuffer;
	VkDeviceMemory				 m_indexBufferMemory;
	std::vector<Vertex>			 m_vertices;
	std::vector<uint32_t>		 m_indices;
	VkBuffer					 m_vertexBuffer;
	VkDeviceMemory				 m_vertexBufferMemory;
	VkCommandPool				 m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;

	void createCommandPool(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Surface& surface);
	void createCommandBuffers(LogicalDevice& logicalDevice, DescriptorSet& descriptorSet);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, RenderPass& renderPass, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, DescriptorSet& descriptorSet);
	auto beginSingleTimeCommands(LogicalDevice& logicalDevice) -> VkCommandBuffer;
	void endSingleTimeCommands(LogicalDevice& logicalDevice, VkCommandBuffer commandBuffer);

	// Allow us to use CommandPool as a pointer
	auto operator*() -> VkCommandPool& { return m_commandPool; }

}; // class CommandPool