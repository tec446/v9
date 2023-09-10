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
#include "RenderPass.h"
#include "DescriptorPool.h"

class Pipeline
{
public:
	const Device*    m_device;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline		 m_pipeline;

	std::vector<VkBuffer>		m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*>			m_uniformBuffersMapped;

	Pipeline(
		const Device* device,
		const SwapChain* swapChain,
		const RenderPass* renderPass,
		const DescriptorPool* descriptorPool
	);

	void createPipeline(
		const SwapChain* swapChain,
		const RenderPass* renderPass,
		const DescriptorPool* descriptorPool
	);

	std::vector<char> readFile(const std::string& filename);

	auto createShaderModule(const std::vector<char>& code) -> VkShaderModule;

	void createUniformBuffers(Device& device, int maxFramesInFlight);
	void createVertexBuffer(Device& device, CommandPool& commandPool);
	void createIndexBuffer(Device& device, CommandPool& commandPool);
	void vkUpdateUniformBuffer(SwapChain& swapChain, uint32_t currentImage);

	// Allow use as a pointer
	auto operator*() -> VkPipeline& { return m_pipeline; }

}; // class Pipeline