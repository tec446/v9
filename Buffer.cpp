#include "Buffer.h"

namespace Buffer
{

void Buffer::create
(
	PhysicalDevice& physicalDevice,
	LogicalDevice& logicalDevice,
	VkDeviceSize		  size,
	VkBufferUsageFlags	  usage,
	VkMemoryPropertyFlags properties,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory
)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(*logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(*logicalDevice, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = SwapChain::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(*logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(*logicalDevice, buffer, bufferMemory, 0);

} // createBuffer()
void Buffer::copy
(
	LogicalDevice& logicalDevice,
	CommandPool& commandPool,
	VkBuffer srcBuffer,
	VkBuffer dstBuffer,
	VkDeviceSize size
)
{
	VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands(logicalDevice);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	commandPool.endSingleTimeCommands(logicalDevice, commandBuffer);
} // copyBuffer()

void Buffer::copyToImage
(
	LogicalDevice& logicalDevice,
	CommandPool& commandPool,
	VkBuffer buffer,
	VkImage image,
	uint32_t width,
	uint32_t height
)
{
	VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands(logicalDevice);

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

	commandPool.endSingleTimeCommands(logicalDevice, commandBuffer);
} // copyBufferToImage()

} // namespace Buffer