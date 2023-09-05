#include "Window.h"

void Window::createSurface(VkInstance& instance)
{
	if (glfwCreateWindowSurface(instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
	{ throw std::runtime_error("failed to create window surface!"); }

} // Surface::createSurface()

void Window::initWindow(GLFWframebuffersizefun framebufferResizeCallback)
{
	glfwInit();
	// don't create an openGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// allows resizing of the window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	// create the window (width, height, name, monitor, openGL specific value)
	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

} // void initWindow()
