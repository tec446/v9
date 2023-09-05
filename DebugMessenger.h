#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include "Config.h"

class DebugMessenger
{
private:
	VkDebugUtilsMessengerEXT m_debugMessenger;

	static auto debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) -> VKAPI_ATTR VkBool32 VKAPI_CALL;

	VkResult CreateDebugUtilsMessengerEXT(VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
public:
	void setupDebugMessenger(VkInstance& instance);
	void DestroyDebugUtilsMessengerEXT(VkInstance& instance, const VkAllocationCallbacks* pAllocator);
	
	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	// Allows us to use the class like a pointer
	VkDebugUtilsMessengerEXT& operator*() { return(m_debugMessenger); }


}; // class DebugMessenger