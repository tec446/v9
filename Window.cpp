#include "Window.h"

void Window::initWindow() 
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

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
} // framebufferResizeCallback()
