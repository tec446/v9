#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <functional>

#include "Instance.h"

class Window
{
public:
	GLFWwindow*  m_window;
	VkSurfaceKHR m_surface;

	void initWindow(GLFWframebuffersizefun framebufferResizeCallback);
	void createSurface(VkInstance& instance);

	// Allows us to use the class like a pointer
	GLFWwindow* operator*() { return m_window; }

private:
	const uint32_t WIDTH{ 800 };
	const uint32_t HEIGHT{ 600 };

}; // class Window