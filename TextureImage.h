#pragma once

#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <fstream>
#include <stdexcept>

#include "ModelLoader.h"

class TextureImage
{
public:
	uint32_t	   m_mipLevels;
	VkImage		   m_textureImage;
	VkDeviceMemory m_textureImageMemory;
	VkImageView    m_textureImageView;
	VkSampler	   m_textureSampler;


	void createTextureImage(LogicalDevice& logicalDevice, SwapChain& swapChain, ModelLoader& modelLoader, UniformBuffers& uniformBuffers);
	void createTextureImageView();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);



}; // class TextureImage