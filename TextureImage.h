#pragma once

#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <fstream>
#include <stdexcept>

#include "TempMagicValues.h"
#include "Device.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "SwapChain.h"

class TextureImage
{
public:
	uint32_t	   m_mipLevels;
	//VkImage		   m_textureImage;
	//VkDeviceMemory m_textureImageMemory;
	//VkImageView    m_textureImageView;
	//VkSampler	   m_textureSampler;

	void createTextureImage(Device& device, CommandPool& commandPool, SwapChain& swapChain);
	void createTextureImageView(Device& device, SwapChain& swapChain);
	void transitionImageLayout(Device& device, CommandPool& commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void generateMipmaps(Device& device, CommandPool& commandPool, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	void createTextureSampler(Device& device);

	// Allows us to use the class like a pointer
	auto operator*() -> VkImage& { return(m_textureImage); } // operator * override

}; // class TextureImage