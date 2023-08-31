#pragma once

#include <vulkan/vulkan.h>
#include <array>
#include <vector>
#include <stdexcept>

#include "LogicalDevice.h"
#include "UniformBuffers.h"
#include "TextureImage.h"

class DescriptorSet
{
public:
	const int MAX_FRAMES_IN_FLIGHT = 2;

	VkDescriptorSetLayout		 m_descriptorSetLayout{};
	VkDescriptorPool			 m_descriptorPool{};
	std::vector<VkDescriptorSet> m_descriptorSets{};

	void createDescriptorSetLayout(LogicalDevice& logicalDevice);
	void createDescriptorSets(LogicalDevice& logicalDevice, UniformBuffers& uniformBuffer, TextureImage& textureImage);
	void createDescriptorPool(LogicalDevice&);

	// Allows us to use DescriptorSet as a pointer
	auto operator*() -> VkDescriptorSetLayout& { return m_descriptorSetLayout; }

}; // class DescriptorSet