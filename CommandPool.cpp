#include "CommandPool.h"

#include <iostream>

#include "RenderObjects.h"

void CommandPool::createCommandPool(Device& device, VkSurfaceKHR& surface)
{
	QueueFamilyIndices queueFamilyIndices = device.findQueueFamilies(device.m_physical, surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(*device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics command pool!");
	}
} // createCommandPool()

void CommandPool::createCommandBuffers(Device& device, int maxFramesInFlight)
{
	m_commandBuffers.resize(maxFramesInFlight);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool;
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

	if (vkAllocateCommandBuffers(*device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!"); }

} // createCommandBuffers()

void CommandPool::recordCommandBuffer(
	VkCommandBuffer				  commandBuffer,
	VkPipeline&					  graphicsPipeline,
	VkPipelineLayout&			  graphicsPipelineLayout,
	VkRenderPass&				  renderPass,
	uint32_t					  imageIndex,
	std::vector<VkFramebuffer>&   frameBuffers,
	VkExtent2D					  extent2d,
	RenderObjects&				  renderObjects
) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!"); }

	VkRenderPassBeginInfo renderPassInfo{}; {
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = frameBuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = extent2d;
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();
	} // renderPassInfo

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(extent2d.width);
		viewport.height = static_cast<float>(extent2d.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0,0 };
		scissor.extent = extent2d;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


		// If there is anything to render
		if (renderObjects.m_instances.referenceIndices.size() > 0) {
			auto& objects  { renderObjects.m_objects   };
			auto& instances{ renderObjects.m_instances };
			auto& objectIndices{objects.objectIndices };
			auto& referenceIndices{instances.referenceIndices};
			
			for (int i = 0; i < referenceIndices.size(); i++) {
				// vertexBuffer
				uint16_t vertexBufferIndex{ static_cast<uint16_t>(RenderObjects::Objects::IndexType::vertexBuffer) };
				VkBuffer& vertexBuffer{ objects.vertexBuffer[objectIndices[referenceIndices[i]][vertexBufferIndex]] };
				// indexBuffer
				uint16_t indexBufferIndex{ static_cast<uint16_t>(RenderObjects::Objects::IndexType::indexBuffer) };
				VkBuffer& indexBuffer{ objects.indexBuffer[objectIndices[referenceIndices[i]][indexBufferIndex]] };
				// indices
				uint16_t indicesIndex{ static_cast<uint16_t>(RenderObjects::Objects::IndexType::indexBuffer) };
				std::vector<uint32_t>& indices{ objects.indices[objectIndices[referenceIndices[i]][indicesIndex]] };
				// descriptorSets
				std::vector<VkDescriptorSet>& descriptorSets{ instances.descriptorSets[i] };

				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);
				vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

				vkCmdBindDescriptorSets(
					commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					graphicsPipelineLayout, 
					0, 
					1, 
					&descriptorSets[m_currentFrame],
					0,
					nullptr
				);

				vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

			} // for each instance
		} // if instances to render
	}
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!"); }

} // recordCommandBuffer()

auto CommandPool::beginSingleTimeCommands(Device& device) -> VkCommandBuffer
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(*device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
} // beginSingleTimeCommands()

void CommandPool::endSingleTimeCommands(Device& device, VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(device.m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(device.m_graphicsQueue);

	vkFreeCommandBuffers(*device, m_commandPool, 1, &commandBuffer);
} // endSingleTimeCommands()

void CommandPool::createSyncObjects
(
	VkDevice& logicalDevice,
	const int maxFramesInFlight
)
{
	m_imageAvailableSemaphores.resize(maxFramesInFlight);
	m_renderFinishedSemaphores.resize(maxFramesInFlight);
	m_inFlightFences.resize(maxFramesInFlight);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (size_t i = 0; i < maxFramesInFlight; i++) {
		if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
} // createSyncObjects()
