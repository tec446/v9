#pragma once

#include <vulkan/vulkan.h>

#include "CommandPool.h"

class SyncObjects
{
public:



	void createSyncObjects(VkDevice logicalDevice, CommandPool& commandPool, DescriptorSet& descriptorSet);

}; // class SyncObjects