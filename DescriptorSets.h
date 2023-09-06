#pragma once

#include <vulkan/vulkan.h>
#include <array>
#include <vector>
#include <stdexcept>

#include "Types.h"

class DescriptorSets
{
public:
	const int m_maxFramesInFlight = 2;

	VkDescriptorSetLayout		 m_descriptorSetLayout{};
	VkDescriptorPool			 m_descriptorPool{};
	std::vector<VkDescriptorSet> m_descriptorSets{};

	void createDescriptorSetLayout(VkDevice& logicalDevice);
	void createDescriptorPool(VkDevice& logicalDevice);
	void createDescriptorSets(VkDevice& logicalDevice, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler);

	// Allows us to use DescriptorSet as a pointer
	auto operator*()		   -> std::vector<VkDescriptorSet>& { return m_descriptorSets; }
	auto operator[](int index) -> VkDescriptorSet& { return m_descriptorSets[index]; }

}; // class DescriptorSet