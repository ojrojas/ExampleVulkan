#pragma once
#include "VulkanInstance.hpp"

namespace EngineVulkan
{
	class Application
	{
	public:
		void Run();
		void InitVulkan(int width, int height, std::string windowName);
		void MainLoop();
		void CleanUp();
	private:
		VulkanInstance _vulkanInstance;
	};
}