#pragma once

#include <vulkan/vulkan.h>
#include <array>
#include <vector>
#include <stdexcept>

#include "Types.h"

class DescriptorSet
{
public:
	const int MAX_FRAMES_IN_FLIGHT = 2;

	VkDescriptorSetLayout		 m_descriptorSetLayout{};
	VkDescriptorPool			 m_descriptorPool{};
	//std::vector<VkDescriptorSet> m_descriptorSets{};

	void createDescriptorSetLayout(VkDevice& logicalDevice);
<<<<<<< Updated upstream:DescriptorSet.h
	void createDescriptorSets(VkDevice& logicalDevice, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler);
	void createDescriptorPool(VkDevice&);

	// Allows us to use DescriptorSet as a pointer
	auto operator*()		   -> std::vector<VkDescriptorSet>& { return m_descriptorSets; }
	auto operator[](int index) -> VkDescriptorSet&				{ return m_descriptorSets[index]; }

=======
	void createDescriptorPool(VkDevice& logicalDevice);
	void createDescriptorSets(VkDevice& logicalDevice, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler, std::vector<VkDescriptorSet>& descriptorSets);
	
>>>>>>> Stashed changes:DescriptorPool.h
}; // class DescriptorSet