#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <memory>

#include "Config.h"
#include "DebugMessenger.h"

class Instance
{
private:
	VkInstance m_instance;

public:
	void createInstance(DebugMessenger debugMessenger);
	static bool checkValidationLayerSupport();
	static auto getRequiredExtensions() -> std::vector<const char*>;

	// Allows us to use the class like a pointer
	auto operator*() -> VkInstance& { return(m_instance); } // operator * override

}; // class Instance