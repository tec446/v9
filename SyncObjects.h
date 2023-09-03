#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

class SyncObjects
{
public:
	void createSyncObjects(VkDevice& logicalDevice, std::vector<VkSemaphore>& imageAvailableSemaphores, std::vector<VkSemaphore>& renderFinishedSemaphores, std::vector<VkFence>& inFlightFences, const int maxFramesInFlight);

}; // class SyncObjects