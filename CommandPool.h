#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <stdexcept>

#include "Device.h"
#include "RenderObjects.h"

class CommandPool
{
public:
	std::vector<VkSemaphore>	 m_imageAvailableSemaphores;
	std::vector<VkSemaphore>	 m_renderFinishedSemaphores;
	std::vector<VkFence>		 m_inFlightFences;
	uint32_t					 m_currentFrame = 0;
	//VkBuffer					 m_vertexBuffer;
	//VkDeviceMemory				 m_vertexBufferMemory;
	//VkBuffer					 m_indexBuffer;
	//VkDeviceMemory				 m_indexBufferMemory;
	//std::vector<Vertex>			 m_vertices;
	//std::vector<uint32_t>		 m_indices;
	VkCommandPool				 m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;

	void createSyncObjects(VkDevice& logicalDevice, const int maxFramesInFlight);
	void createCommandPool(Device& device, VkSurfaceKHR& surface);
	void createCommandBuffers(Device& device, int maxFramesInFlight);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, VkPipeline& graphicsPipeline, VkPipelineLayout& graphicsPipelineLayout, VkRenderPass& renderPass, uint32_t imageIndex, std::vector<VkFramebuffer>& frameBuffers, VkExtent2D extent2d, RenderObjects& renderObjects);
	auto beginSingleTimeCommands(Device& device) -> VkCommandBuffer;
	void endSingleTimeCommands(Device& device, VkCommandBuffer commandBuffer);

	// Allow us to use CommandPool as a pointer
	auto operator*() -> VkCommandPool& { return m_commandPool; }

}; // class CommandPool