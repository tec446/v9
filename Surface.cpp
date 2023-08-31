#include "Surface.h"

void Surface::createSurface(Instance& instance, Window& window)
{
	if (glfwCreateWindowSurface(*instance, *window, nullptr, &m_surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}

} // Surface::createSurface()
