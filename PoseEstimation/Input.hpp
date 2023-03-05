#pragma once
#include "Libs.hpp"

enum Button
{
	LEFT, 
	RIGHT,
	MIDDLE,
	NONE
};

class Input
{
public:
	static Button GetPressedButton(GLFWwindow* window)
	{
		if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
			return Button::LEFT;

		if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
			return Button::RIGHT;

		if (glfwGetMouseButton(window, 2) == GLFW_PRESS)
			return Button::MIDDLE;

		return Button::NONE;
	}
};