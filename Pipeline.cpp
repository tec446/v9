#include "Pipeline.h"

Pipeline::Pipeline(
	const Device* device,
	const SwapChain* swapChain,
	const RenderPass* renderPass,
	const DescriptorPool* descriptorPool
) : m_device{ device }
{
	createPipeline(swapChain, renderPass, descriptorPool);
} // TestPipeline()

void Pipeline::createPipeline(
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

	auto bindingDescription = Vertex::getBindingDescription();
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
		throw std::runtime_error("failed to create pipeline layout!");
	}

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
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	// shaders are loaded and the modules are no longer required
	vkDestroyShaderModule(m_device->m_logical, fragShaderModule, nullptr);
	vkDestroyShaderModule(m_device->m_logical, vertShaderModule, nullptr);

} // createGraphicsPipeline()

std::vector<char> Pipeline::readFile(const std::string& filename)
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
}

auto Pipeline::createShaderModule(const std::vector<char>& code) -> VkShaderModule {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(m_device->m_logical, &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
		throw std::runtime_error("failed to create shader module!"); }

	return shaderModule;
} // createShaderModule()

void Pipeline::createUniformBuffers(
	Device& device,
	int maxFramesInFlight
) {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_uniformBuffers.resize(maxFramesInFlight);
	m_uniformBuffersMemory.resize(maxFramesInFlight);
	m_uniformBuffersMapped.resize(maxFramesInFlight);

	for (size_t i = 0; i < maxFramesInFlight; i++)
	{
		Buffer::create(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);

		vkMapMemory(*device, m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
	}
} // createUniformBuffers()

void Pipeline::createVertexBuffer(
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

void Pipeline::createIndexBuffer(
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

void Pipeline::vkUpdateUniformBuffer(SwapChain& swapChain, uint32_t currentImage) {
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
