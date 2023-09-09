#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <glm/fwd.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <chrono>

#include "TempMagicValues.h"
#include "Device.h"
#include "IO.h"
#include "Buffer.h"

class Pipeline
{
public:
	VkPipelineLayout m_pipelineLayout{};
	VkPipeline		 m_pipeline{};

	std::vector<VkBuffer>		m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*>			m_uniformBuffersMapped;

	void createGraphicsPipeline(
		Device& device, 
		VkDescriptorSetLayout& descriptorSetLayout, 
		VkRenderPass& renderPass//,
//		VkPipelineLayout& pipelineLayout, 
//		VkPipeline& pipeline
	);
	static auto createShaderModule(
		Device& device, 
		const std::vector<char>& code
	) -> VkShaderModule;

	void createUniformBuffers(Device& device, int maxFramesInFlight);
	void createVertexBuffer(Device& device, CommandPool& commandPool);
	void createIndexBuffer(Device& device, CommandPool& commandPool);
	void vkUpdateUniformBuffer(SwapChain& swapChain, uint32_t currentImage);

	// Allow use as a pointer
	auto operator*() -> VkPipeline& { return m_pipeline; }

}; // class Pipeline