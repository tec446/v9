#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <stdexcept>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"

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
	VkCommandPool				 m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;

	void createCommandPool(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VkSurfaceKHR& surface);
	void createCommandBuffers(LogicalDevice& logicalDevice, int maxFramesInFlight);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, std::vector<VkDescriptorSet>& descriptorSets, VkPipeline& graphicsPipeline, VkPipelineLayout& graphicsPipelineLayout, VkRenderPass& renderPass, uint32_t imageIndex, std::vector<VkFramebuffer>& frameBuffers, VkExtent2D extent2d);
	auto beginSingleTimeCommands(LogicalDevice& logicalDevice) -> VkCommandBuffer;
	void endSingleTimeCommands(LogicalDevice& logicalDevice, VkCommandBuffer commandBuffer);

	// Allow us to use CommandPool as a pointer
	auto operator*() -> VkCommandPool& { return m_commandPool; }

}; // class CommandPool