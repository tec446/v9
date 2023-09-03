#include "UniformBuffers.h"

void UniformBuffers::createUniformBuffers
(
	PhysicalDevice& physicalDevice, 
	LogicalDevice& logicalDevice, 
	int maxFramesInFlight
)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_uniformBuffers.resize(maxFramesInFlight);
	m_uniformBuffersMemory.resize(maxFramesInFlight);
	m_uniformBuffersMapped.resize(maxFramesInFlight);

	for (size_t i = 0; i < maxFramesInFlight; i++)
	{
		Buffer::create(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);

		vkMapMemory(*logicalDevice, m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
	}
} // createUniformBuffers()

void UniformBuffers::createVertexBuffer
(
	PhysicalDevice& physicalDevice, 
	LogicalDevice&  logicalDevice, 
	CommandPool&	commandPool
)
{
	VkDeviceSize bufferSize = sizeof(commandPool.m_vertices[0]) * commandPool.m_vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Buffer::create(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, commandPool.m_vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(*logicalDevice, stagingBufferMemory);

	Buffer::create(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.m_vertexBuffer, commandPool.m_vertexBufferMemory );

	Buffer::copy(logicalDevice, commandPool, stagingBuffer, commandPool.m_vertexBuffer, bufferSize);

	vkDestroyBuffer(*logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(*logicalDevice, stagingBufferMemory, nullptr);
} // createVertexBuffer()

void UniformBuffers::createIndexBuffer
(
	PhysicalDevice& physicalDevice, 
	LogicalDevice&  logicalDevice, 
	CommandPool&	commandPool
)
{
	VkDeviceSize bufferSize = sizeof(commandPool.m_indices[0]) * commandPool.m_indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Buffer::create(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, commandPool.m_indices.data(), (size_t)bufferSize);
	vkUnmapMemory(*logicalDevice, stagingBufferMemory);

	Buffer::create(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, commandPool.m_indexBuffer, commandPool.m_indexBufferMemory);

	Buffer::copy(logicalDevice, commandPool, stagingBuffer, commandPool.m_indexBuffer, bufferSize);

	vkDestroyBuffer(*logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(*logicalDevice, stagingBufferMemory, nullptr);
} // createIndexBuffer()

void UniformBuffers::vkUpdateUniformBuffer(SwapChain& swapChain, uint32_t currentImage) {
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



