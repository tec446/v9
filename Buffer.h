#pragma once

#include <vulkan/vulkan.h>

class Device;
class CommandPool;
class SwapChain;

namespace Buffer
{
	void create(Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copy(Device& device, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyToImage(Device& device, CommandPool& commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

}; // class Buffer