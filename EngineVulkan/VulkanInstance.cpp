#include "VulkanInstance.hpp"

namespace EngineVulkan
{
	void VulkanInstance::Initialize(int width, int height, std::string windowName)
	{
		InitializeWindow(width, height, windowName);
		InitializeVulkan();
	}

	void VulkanInstance::InitializeWindow(int width, int height, std::string windowName)
	{
		if (glfwInit() != GLFW_TRUE)
			throw std::runtime_error("Error initialized glfw framework create window");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

	}

	void VulkanInstance::InitializeVulkan()
	{
		CreateInstance();
	}

	void VulkanInstance::CreateInstance()
	{
		VkApplicationInfo applicationInfo{};
	}
}