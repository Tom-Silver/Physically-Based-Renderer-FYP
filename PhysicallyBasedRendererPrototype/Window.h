#pragma once

namespace TSFYP
{
	struct Window
	{
		GLFWwindow* glfwWindow;
		unsigned short width, height;
		bool isFullscreen;
	};
}