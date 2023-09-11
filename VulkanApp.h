#pragma once

#include <unordered_map>
#include <tiny_obj_loader.h>
//#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <chrono>
#include <array>
#include <fstream>
#include <set>
#include <optional>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <algorithm>

#include "TempMagicValues.h"

#include "Config.h"
#include "Types.h"
#include "Instance.h"
#include "Window.h"
#include "DebugMessenger.h"
#include "Device.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "DescriptorPool.h"
#include "IO.h"
#include "Pipeline.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "TextureImage.h"

#include "RenderObjects.h"

const uint32_t WIDTH{ 800 };
const uint32_t HEIGHT{ 600 };
////////////////////////////////////////////////////////////////////////////
class TestPipeline {
public:
	//VkPipelineLayout m_pipelineLayout{};
	//VkPipeline		 m_pipeline{};

	std::vector<VkBuffer>		m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*>			m_uniformBuffersMapped;

	const Device* m_device;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_pipeline;

	TestPipeline(
		const Device* device,
		const SwapChain* swapChain,
		const RenderPass* renderPass,
		const DescriptorPool* descriptorPool
	) : m_device{ device }
	{
		createPipeline(swapChain, renderPass, descriptorPool);
	} // TestPipeline()

	void createPipeline(
		const SwapChain* swapChain,
		const RenderPass* renderPass,
		const DescriptorPool* descriptorPool
	) {
		// get shader code & convert it
		auto vertShaderCode = readFile(TempMagicValues::VERT_SHADER_PATH);
		auto fragShaderCode = readFile(TempMagicValues::FRAG_SHADER_PATH);
		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{}; {
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";
		} // vertShaderStageInfo

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{}; {
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";
		} // fragShaderStageInfo

		// info for all shader stages
		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		auto bindingDescription    = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		// info for how vertex data is loaded into the vertex shader
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; {
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		} // vertexInputInfo

		// info on what kind of geometry will be drawn, IE: triangles, lines, etc...
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{}; {
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;
		} // inputAssembly

		// get swap extent from swap chain, needed for viewport
		const VkExtent2D& swapExtent{ swapChain->m_swapChainExtent };

		// info on viewport size, depth, and offset
		VkViewport viewport{}; {
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)swapExtent.width;
			viewport.height = (float)swapExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
		} // viewport

		// info about clipping
		VkRect2D scissor{}; {
			scissor.offset = { 0, 0 };
			scissor.extent = swapExtent;
		} // scissor

		// combined info on viewport and scissor
		VkPipelineViewportStateCreateInfo viewportState{}; {
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;
		} // viewportState

		// rasterization stage info
		VkPipelineRasterizationStateCreateInfo rasterizer{}; {
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
		} // rasterizer

		// multisampling info
		VkPipelineMultisampleStateCreateInfo multisampling{}; {
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		} // multisampling

		// depth stencil
		VkPipelineDepthStencilStateCreateInfo depthStencil{}; {
			depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencil.depthTestEnable = VK_TRUE;
			depthStencil.depthWriteEnable = VK_TRUE;
			depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
			depthStencil.depthBoundsTestEnable = VK_FALSE;
			depthStencil.minDepthBounds = 0.0f; // optional
			depthStencil.maxDepthBounds = 1.0f; // optional
			depthStencil.stencilTestEnable = VK_FALSE;
			depthStencil.front = {};	// optional
			depthStencil.back = {};	//optional
		} // depthStencil

		// color blending info
		VkPipelineColorBlendAttachmentState colorBlendAttachment{}; {
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
				| VK_COLOR_COMPONENT_G_BIT
				| VK_COLOR_COMPONENT_B_BIT
				| VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
		} // colorBlendAttachment

		VkPipelineColorBlendStateCreateInfo colorBlending{}; {
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;
		} // colorBlending

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantData);

		// pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorPool->m_descriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(m_device->m_logical, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!"); }

		// Pipeline creation info
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = renderPass->m_renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(m_device->m_logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!"); }

		// shaders are loaded and the modules are no longer required
		vkDestroyShaderModule(m_device->m_logical, fragShaderModule, nullptr);
		vkDestroyShaderModule(m_device->m_logical, vertShaderModule, nullptr);

	} // createGraphicsPipeline()

	std::vector<char> readFile(const std::string& filename)
	{
		// read in file and start from the end to get file size
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		// throw error if file wasn't opened successfully
		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();	// get file size
		std::vector<char> buffer(fileSize);		// create buffer to hold code
		file.seekg(0);							// start from beginning of file
		file.read(buffer.data(), fileSize);		// copy data to buffer
		file.close();

		return buffer;
	} // readFile()

	VkShaderModule createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_device->m_logical, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!"); }

		return shaderModule;
	} // createShaderModule()

	void createUniformBuffers(
		Device& device,
		int maxFramesInFlight
	)
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_uniformBuffers.resize(maxFramesInFlight);
		m_uniformBuffersMemory.resize(maxFramesInFlight);
		m_uniformBuffersMapped.resize(maxFramesInFlight);

		for (size_t i = 0; i < maxFramesInFlight; i++) {
			Buffer::create(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);
			vkMapMemory(*device, m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
		}
	} // createUniformBuffers()

	void createVertexBuffer(
		Device& device,
		CommandPool& commandPool
	) {
		VkDeviceSize bufferSize = sizeof(commandPool.m_vertices[0]) * commandPool.m_vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Buffer::create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, commandPool.m_vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(*device, stagingBufferMemory);

		Buffer::create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.m_vertexBuffer, commandPool.m_vertexBufferMemory);

		Buffer::copy(device, commandPool, stagingBuffer, commandPool.m_vertexBuffer, bufferSize);

		vkDestroyBuffer(*device, stagingBuffer, nullptr);
		vkFreeMemory(*device, stagingBufferMemory, nullptr);
	} // createVertexBuffer()

	void createIndexBuffer(
		Device& device,
		CommandPool& commandPool
	) {
		VkDeviceSize bufferSize = sizeof(commandPool.m_indices[0]) * commandPool.m_indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Buffer::create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, commandPool.m_indices.data(), (size_t)bufferSize);
		vkUnmapMemory(*device, stagingBufferMemory);

		Buffer::create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.m_indexBuffer, commandPool.m_indexBufferMemory);

		Buffer::copy(device, commandPool, stagingBuffer, commandPool.m_indexBuffer, bufferSize);

		vkDestroyBuffer(*device, stagingBuffer, nullptr);
		vkFreeMemory(*device, stagingBufferMemory, nullptr);
	} // createIndexBuffer()

	void vkUpdateUniformBuffer(SwapChain& swapChain, uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>
			(currentTime - startTime).count();

		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f),
			swapChain.m_swapChainExtent.width / (float)swapChain.m_swapChainExtent.height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		memcpy(m_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	} // vkUpdateUniformBuffer()

}; // class TestPipeline
////////////////////////////////////////////////////////////////////////////
class HelloTriangleApplication {
public:
	HelloTriangleApplication() = default;

	void run() {
		m_window.initWindow(framebufferResizeCallback);
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	// Wrappers
	DebugMessenger m_debugMessenger;
	Instance       m_instance;
	Device         m_device;
	Window		   m_window;
	SwapChain	   m_swapChain;
	RenderPass     m_renderPass;
	DescriptorPool m_descriptorPool;
	CommandPool    m_commandPool;
	TextureImage   m_textureImage;
	std::unique_ptr<TestPipeline> m_pipeline;

	RenderObjects m_renderObjects{};
	RenderObjectInstances m_renderObjectInstances{};
	//
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window)) -> m_framebufferResized = true;
	};

	bool m_framebufferResized = false;
	
//	Pipeline m_pipeline;
	//////////////////////////
	void initVulkan() {
		m_instance.createInstance(m_debugMessenger);
		m_debugMessenger.setupDebugMessenger(*m_instance);
		m_window.createSurface(*m_instance);
		m_device.pickPhysicalDevice(*m_instance, m_window.m_surface);
		m_device.createLogicalDevice(validationLayers, m_window.m_surface);
		m_swapChain.createSwapChain(m_device, m_window.m_surface, **m_window);
		m_swapChain.createImageViews(m_device);
		m_renderPass.createRenderPass(m_device, m_swapChain);
		m_descriptorPool.createDescriptorSetLayout(*m_device);
		m_pipeline = std::make_unique<TestPipeline>(&m_device, &m_swapChain, &m_renderPass, &m_descriptorPool);
		m_commandPool.createCommandPool(m_device, m_window.m_surface);
		m_swapChain.createColorResources(m_device, m_swapChain);
		m_swapChain.createDepthResources(m_device, m_swapChain);
		m_swapChain.createFramebuffers(m_device, *m_renderPass);

		/*
		/// TODO: RenderObject Specific

		m_textureImage.createTextureImage(m_device, m_commandPool, m_swapChain);
		m_textureImage.createTextureImageView(m_device, m_swapChain);
		m_textureImage.createTextureSampler(m_device);
		loadModel(m_commandPool.m_vertices, m_commandPool.m_indices);
		m_pipeline->createVertexBuffer(m_device, m_commandPool);
		m_pipeline->createIndexBuffer(m_device, m_commandPool);
		m_pipeline->createUniformBuffers(m_device, m_descriptorPool.m_maxFramesInFlight);
		*/

		m_descriptorPool.createDescriptorPool(*m_device);
		/// TODO: RenderObject Specific
		//m_descriptorPool.createDescriptorSets(*m_device, m_pipeline->m_uniformBuffers, m_textureImage.m_textureImageView, m_textureImage.m_textureSampler);
		m_commandPool.createCommandBuffers(m_device, m_descriptorPool.m_maxFramesInFlight);
		m_commandPool.createSyncObjects(*m_device, m_descriptorPool.m_maxFramesInFlight);
	} // initVulkan()

	void temporaryCreateObject() {

		m_textureImage.createTextureImage(m_device, m_commandPool, m_swapChain);
		m_textureImage.createTextureImageView(m_device, m_swapChain);
		m_textureImage.createTextureSampler(m_device);
		loadModel(m_commandPool.m_vertices, m_commandPool.m_indices);
		m_pipeline->createVertexBuffer(m_device, m_commandPool);
		m_pipeline->createIndexBuffer(m_device, m_commandPool);
		m_pipeline->createUniformBuffers(m_device, m_descriptorPool.m_maxFramesInFlight);
		m_descriptorPool.createDescriptorSets(*m_device, m_pipeline->m_uniformBuffers, m_textureImage.m_textureImageView, m_textureImage.m_textureSampler);

	} // createObject()

	void mainLoop() {
		while (!glfwWindowShouldClose(*m_window)) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(*m_device);

	} // mainLoop()

	void cleanup() {
		m_swapChain.cleanupSwapChain(*m_device);

		vkDestroySampler(*m_device, m_textureImage.m_textureSampler, nullptr);
		vkDestroyImageView(*m_device, m_textureImage.m_textureImageView, nullptr);
		vkDestroyImage(*m_device, m_textureImage.m_textureImage, nullptr);
		vkFreeMemory(*m_device, m_textureImage.m_textureImageMemory, nullptr);

		// TODO: make this apply to renderObjects
		for (size_t i = 0; i < m_descriptorPool.m_maxFramesInFlight; i++)
		{
			vkDestroyBuffer(*m_device, m_pipeline->m_uniformBuffers[i], nullptr);
			vkFreeMemory(*m_device, m_pipeline->m_uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(*m_device, m_descriptorPool.m_descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(*m_device, m_descriptorPool.m_descriptorSetLayout, nullptr);

		vkDestroyBuffer(*m_device, m_commandPool.m_indexBuffer, nullptr);
		vkFreeMemory(*m_device, m_commandPool.m_indexBufferMemory, nullptr);

		vkDestroyBuffer(*m_device, m_commandPool.m_vertexBuffer, nullptr);
		vkFreeMemory(*m_device, m_commandPool.m_vertexBufferMemory, nullptr);

		vkDestroyPipeline(*m_device, m_pipeline->m_pipeline, nullptr);
		vkDestroyPipelineLayout(*m_device, m_pipeline->m_pipelineLayout, nullptr);

		vkDestroyRenderPass(*m_device, *m_renderPass, nullptr);

		for (size_t i = 0; i < m_descriptorPool.m_maxFramesInFlight; i++) {
			vkDestroySemaphore(*m_device, m_commandPool.m_imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(*m_device, m_commandPool.m_renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(*m_device, m_commandPool.m_inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(*m_device, *m_commandPool, nullptr);

		vkDestroyDevice(*m_device, nullptr);

		if (enableValidationLayers) 
		{ m_debugMessenger.DestroyDebugUtilsMessengerEXT(*m_instance, nullptr); }

		vkDestroySurfaceKHR(*m_instance, m_window.m_surface, nullptr);
		vkDestroyInstance(*m_instance, nullptr);

		glfwDestroyWindow(*m_window);

		glfwTerminate();

	} // cleanup()

	bool hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void drawFrame() {
		vkWaitForFences(*m_device, 1, &m_commandPool.m_inFlightFences[m_commandPool.m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(*m_device, *m_swapChain, UINT64_MAX,
			m_commandPool.m_imageAvailableSemaphores[m_commandPool.m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			m_swapChain.recreateSwapChain(m_device, **m_window, m_window.m_surface, *m_renderPass);
			return;
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		// Only reset fence when submitting work
		vkResetFences(*m_device, 1, &m_commandPool.m_inFlightFences[m_commandPool.m_currentFrame]);

		// TODO: fix this
		//m_pipeline->vkUpdateUniformBuffer(m_swapChain, m_commandPool.m_currentFrame);
		// TODO: fix this

		vkResetCommandBuffer(m_commandPool.m_commandBuffers[m_commandPool.m_currentFrame], 0);

		for (int i = 0; i < m_renderObjectInstances.m_referenceIndices.size(); i++)
		{
			m_commandPool.recordCommandBuffer(
				m_commandPool.m_commandBuffers[m_commandPool.m_currentFrame], 
				m_renderObjects.m_descriptorSets[i],
				m_pipeline->m_pipeline, 
				m_pipeline->m_pipelineLayout, 
				*m_renderPass, imageIndex, 
				m_swapChain.m_swapChainFramebuffers, 
				m_swapChain.m_swapChainExtent
			);
		} // for (i in renderObjectInstances)
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_commandPool.m_imageAvailableSemaphores[m_commandPool.m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandPool.m_commandBuffers[m_commandPool.m_currentFrame];
		VkSemaphore signalSemaphores[] = { m_commandPool.m_renderFinishedSemaphores[m_commandPool.m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { *m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		if (vkQueueSubmit(m_device.m_graphicsQueue, 1, &submitInfo, m_commandPool.m_inFlightFences[m_commandPool.m_currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!"); }

		result = vkQueuePresentKHR(m_device.m_presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized) {
			m_framebufferResized = false;
			m_swapChain.recreateSwapChain(m_device, **m_window, m_window.m_surface, *m_renderPass);
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image"); 
		}

		m_commandPool.m_currentFrame = (m_commandPool.m_currentFrame + 1) % m_descriptorPool.m_maxFramesInFlight;

	} // drawFrame()

	void loadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, TempMagicValues::MODEL_PATH.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	} // loadModel()

}; // class VulkanApp
