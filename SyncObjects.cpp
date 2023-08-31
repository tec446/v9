#include "SyncObjects.h"


void SyncObjects::createSyncObjects(VkDevice logicalDevice, CommandPool& commandPool, DescriptorSet& descriptorSet)
{
	commandPool.m_imageAvailableSemaphores.resize(descriptorSet.MAX_FRAMES_IN_FLIGHT);
	commandPool.m_renderFinishedSemaphores.resize(descriptorSet.MAX_FRAMES_IN_FLIGHT);
	commandPool.m_inFlightFences.resize(descriptorSet.MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (size_t i = 0; i < descriptorSet.MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &commandPool.m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &commandPool.m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &commandPool.m_inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
} // createSyncObjects()