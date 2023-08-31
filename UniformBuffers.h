#pragma once

#include <vulkan/vulkan.h>
#include <glm/fwd.hpp>
#include <glm/gtx/hash.hpp>
#include <vector>
#include <chrono>

#include "LogicalDevice.h"
#include "GraphicsPipeline.h"
#include "CommandPool.h"


class UniformBuffers
{
public:
	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	std::vector<VkBuffer>		m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*>			m_uniformBuffersMapped;

	void createUniformBuffers(LogicalDevice& logicalDevice);
	void createVertexBuffer(LogicalDevice& logicalDevice);
	void createIndexBuffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void vkUpdateUniformBuffer(uint32_t currentImage);
	void copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	
	// Allows us to use UniformBuffers as a pointer
	auto operator[](int i) -> VkBuffer& { return m_uniformBuffers[i]; }

}; // class UniformBuffers