#include "Application.hpp"

namespace EngineVulkan
{
	void Application::Run()
	{
		MainLoop();
	}

	void Application::InitVulkan(int width, int height, std::string windowName)
	{
		_vulkanInstance.Initialize(width, height, windowName);
	}

	void Application::MainLoop()
	{
		while(!glfwWindowShouldClose(_vulkanInstance.GetWindow()))
		{
			glfwPollEvents();
		}
	}

	void Application::CleanUp()
	{
		glfwDestroyWindow(_vulkanInstance.GetWindow());

		glfwTerminate();
	}
}