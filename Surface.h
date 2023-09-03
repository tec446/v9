#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Instance.h"
#include "Window.h"

class Surface
{
public:
	VkSurfaceKHR m_surface{};
	
	void createSurface(VkInstance& instance, GLFWwindow& window);

	// Allows us to use the class like a pointer
	VkSurfaceKHR& operator*() { return m_surface; }
}; // class Surface