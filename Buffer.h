#pragma once

#include <vulkan/vulkan.h>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "CommandPool.h"
#include "SwapChain.h"

namespace Buffer
{
	void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copy(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyToImage(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

}; // class Buffer