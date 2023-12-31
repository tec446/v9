#include "Buffer.h"

namespace Buffer
{

	void Buffer::create
	(
		Device&				  device,
		VkDeviceSize		  size,
		VkBufferUsageFlags	  usage,
		VkMemoryPropertyFlags properties,
		VkBuffer&			  buffer,
		VkDeviceMemory&		  bufferMemory
	)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(*device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(*device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = SwapChain::findMemoryType(device, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(*device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(*device, buffer, bufferMemory, 0);

	} // createBuffer()
	void Buffer::copy
	(
		Device& device,
		CommandPool& commandPool,
		VkBuffer srcBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize size
	)
	{
		VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands(device);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		commandPool.endSingleTimeCommands(device, commandBuffer);
	} // copyBuffer()

	void Buffer::copyToImage
	(
		Device& device,
		CommandPool& commandPool,
		VkBuffer buffer,
		VkImage image,
		uint32_t width,
		uint32_t height
	)
	{
		VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands(device);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		commandPool.endSingleTimeCommands(device, commandBuffer);
	} // copyBufferToImage()

} // namespace Buffer