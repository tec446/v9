#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Instance.h"
#include "Window.h"

class Surface
{
public:
	void createSurface(Instance& instance, Window& window);

	VkSurfaceKHR m_surface{};

	// Allows us to use the class like a pointer
	VkSurfaceKHR& operator*() { return m_surface; }
}; // class Surface