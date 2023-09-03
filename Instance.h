#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <memory>

#include "vulkanConfig.h"

#include "DebugMessenger.h"

class Instance
{
private:
	VkInstance m_instance;

public:
	const std::vector<const char*> m_validationLayers = { "VK_LAYER_KHRONOS_validation" };

	void createInstance(DebugMessenger debugMessenger);
	bool checkValidationLayerSupport();
	auto getRequiredExtensions() -> std::vector<const char*>;
	
	// Allows us to use the class like a pointer
	auto operator*() -> VkInstance& { return(m_instance); } // operator * override

	// operator * override
}; // class Instance
