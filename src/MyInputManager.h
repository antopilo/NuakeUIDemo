#pragma once
#include "NuakeUI/InputManager.h"

#include <NuakeRenderer/Window.h>
#include <Dependencies/NuakeRenderer/Dependencies/glfw/include/GLFW/glfw3.h>



class MyInputManager : public NuakeUI::InputManager
{
public:
	MyInputManager(NuakeRenderer::Window& window)
	{
		mWindow = window.GetHandle();

		glfwSetScrollCallback(mWindow, MyInputManager::scroll_callback);
	}

	bool IsMouseInputDown() override
	{
		auto state = glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_1);
		return state == GLFW_PRESS;
	}

	float GetMouseX() override
	{
		double xpos, ypos;
		glfwGetCursorPos(mWindow, &xpos, &ypos);
		return xpos;
	}

	float GetMouseY() override
	{
		double xpos, ypos;
		glfwGetCursorPos(mWindow, &xpos, &ypos);
		return ypos;
	}

	float GetScrollX() override
	{
		return ScrollX;
	}

	float GetScrollY() override
	{
		return ScrollY;
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ScrollX = xoffset;
		ScrollY = yoffset;
	}

private:
	GLFWwindow* mWindow;
};