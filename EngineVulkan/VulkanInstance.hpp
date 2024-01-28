#pragma once 
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>


namespace EngineVulkan
{
	class VulkanInstance
	{
	public:
		void Initialize(int width, int height, std::string windowName);
		void InitializeWindow(int width, int height, std::string windowName);
		void InitializeVulkan();

		GLFWwindow* GetWindow() const { return _window; }

	private:

		GLFWwindow* _window;
		std::string _nameWindow;
		int _width;
		int _height;

	};
}