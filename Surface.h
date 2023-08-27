#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Surface
{
public:
	void createSurface(VkInstance& instance, GLFWwindow* window);

	VkSurfaceKHR m_surface{};

	VkSurfaceKHR& operator*();
}; // class Surface