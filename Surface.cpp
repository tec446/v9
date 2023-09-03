#include "Surface.h"

void Surface::createSurface(VkInstance& instance, GLFWwindow& window)
{
	if (glfwCreateWindowSurface(instance, &window, nullptr, &m_surface) != VK_SUCCESS)
	{ throw std::runtime_error("failed to create window surface!"); }

} // Surface::createSurface()
