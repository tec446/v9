#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

#include "TempMagicValues.h"
#include "Device.h"
#include "IO.h"

class Pipeline
{
public:
	VkPipelineLayout m_pipelineLayout;
	VkPipeline		 m_pipeline;

	static void createGraphicsPipeline(
		Device& device, 
		VkDescriptorSetLayout& descriptorSetLayout, 
		VkRenderPass& renderPass,
		VkPipelineLayout& pipelineLayout, 
		VkPipeline& pipeline
	);
	static auto createShaderModule(
		Device& device, 
		const std::vector<char>& code
	) -> VkShaderModule;

	// Allow use as a pointer
	auto operator*() -> VkPipeline& { return m_pipeline; }

}; // class Pipeline