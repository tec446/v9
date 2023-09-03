#pragma once

#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <fstream>
#include <stdexcept>

#include "TempMagicValues.h"
#include "PhysicalDevice.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "SwapChain.h"

class TextureImage
{
public:
	uint32_t	   m_mipLevels;
	VkImage		   m_textureImage;
	VkDeviceMemory m_textureImageMemory;
	VkImageView    m_textureImageView;
	VkSampler	   m_textureSampler;

	void createTextureImage(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, SwapChain& swapChain);
	void createTextureImageView(LogicalDevice& logicalDevice, SwapChain& swapChain);
	void transitionImageLayout(LogicalDevice& logicalDevice, CommandPool& commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void generateMipmaps(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	void createTextureSampler(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	
	// Allows us to use the class like a pointer
	auto operator*() -> VkImage& { return(m_textureImage); } // operator * override

}; // class TextureImage