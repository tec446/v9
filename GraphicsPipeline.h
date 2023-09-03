#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

#include "TempMagicValues.h"
#include "LogicalDevice.h"
#include "IO.h"

class GraphicsPipeline
{
public:

	VkPipelineLayout m_pipelineLayout;
	VkPipeline		 m_graphicsPipeline;

	void createGraphicsPipeline(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout, VkRenderPass& renderPass);
	auto createShaderModule(const std::vector<char>& code, LogicalDevice& logicalDevice) -> VkShaderModule;

	// Allow us to use GraphicsPipeline as a pointer
	auto operator*() -> VkPipeline& { return m_graphicsPipeline; }

}; // class GraphicsPipeline