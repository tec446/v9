#pragma once

#include <vulkan/vulkan.h>
#include <array>
#include <vector>
#include <stdexcept>

#include "Types.h"

class DescriptorPool
{
public:
	const int m_maxFramesInFlight = 2;

	VkDescriptorSetLayout		 m_descriptorSetLayout{};
	VkDescriptorPool			 m_descriptorPool{};
	std::vector<VkDescriptorSet> m_descriptorSets{};

	void createDescriptorSetLayout(VkDevice& logicalDevice);
	void createDescriptorPool(VkDevice& logicalDevice);
	void createDescriptorSets(VkDevice& logicalDevice, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler);

}; // class DescriptorSet