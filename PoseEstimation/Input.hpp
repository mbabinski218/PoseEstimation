#pragma once
#include "Libs.hpp"

enum Button
{
	Left, 
	Right,
	Middle,
	None
};

class Input
{
public:
	static Button GetPressedButton(GLFWwindow* window)
	{
		if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
			return Button::Left;

		if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
			return Button::Right;

		if (glfwGetMouseButton(window, 2) == GLFW_PRESS)
			return Button::Middle;

		return Button::None;
	}
};