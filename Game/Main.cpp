#include <iostream>
#include <EngineVulkan/Application.hpp>
using namespace EngineVulkan;

int main() 
{
	std::cout << "Hello world" << std::endl;
	static constexpr int WIDTH = 1024;
	static constexpr int HEIGHT = 768;
	Application app;

	try
	{
		app.InitVulkan(WIDTH, HEIGHT, "Example vulkan Engine");
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	app.CleanUp();
	return EXIT_SUCCESS;
}