#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
	const uint32_t WIDTH{ 800 };
	const uint32_t HEIGHT{ 600 };

	GLFWwindow* m_window{};

	
	void initWindow();

	// Allows us to use the class like a pointer
	GLFWwindow* operator*() { return m_window; }

}; // class Window