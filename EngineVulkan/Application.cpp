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
		_vulkanInstance.DestroySwapChainApp();
		
		if (_vulkanInstance._enableValidationLayers)
			_vulkanInstance.DestroyDebugUtilsMessengerEXTApp();

		_vulkanInstance.DestroyLogicalDeviceApp();

		_vulkanInstance.DestroySurfaceApp();
		vkDestroyInstance(_vulkanInstance.GetVkInstance(), nullptr);

		glfwDestroyWindow(_vulkanInstance.GetWindow());

		glfwTerminate();
	}
}