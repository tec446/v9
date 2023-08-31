#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include "vulkanConfig.h"
#include "Instance.h"

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
	
	VkResult CreateDebugUtilsMessengerEXT(Instance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
public:



	void setupDebugMessenger(Instance& instance);
	void DestroyDebugUtilsMessengerEXT(Instance& instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	// Allows us to use the class like a pointer
	VkDebugUtilsMessengerEXT& operator*() { return(m_debugMessenger); }


}; // class DebugMessenger
