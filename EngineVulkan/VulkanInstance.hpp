#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include "EngineVulkanTypes.hpp"

namespace EngineVulkan
{
	class VulkanInstance
	{
	public:
		const std::vector<const char *> _validationLayers = {
			"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char *> _deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};

#if NDEBUG
		const bool _enableValidationLayers = false;
#else
		const bool _enableValidationLayers = true;
#endif

		VkDebugUtilsMessengerEXT _debugMessenger = nullptr;

		void Initialize(int width, int height, std::string windowName);
		void InitializeWindow(int width, int height, std::string windowName);
		void InitializeVulkan();

		GLFWwindow *GetWindow() const { return _window; }
		VkInstance GetVkInstance() const { return _instance; }

		void DestroyDebugUtilsMessengerEXTApp()
		{
			DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
		}

		void DestroyLogicalDeviceApp()
		{
			vkDestroyDevice(_device, nullptr);
		}

		void DestroySurfaceApp()
		{
			vkDestroySurfaceKHR(_instance, _surface, nullptr);
		}

	private:
		GLFWwindow *_window = nullptr;
		std::string _nameWindow;
		int _width;
		int _height;

		std::vector<const char *> GetRequiredExtensions();

		VkInstance _instance;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDevice _device;

		VkSurfaceKHR _surface;
		VkQueue _graphicsQueue;
		VkQueue _presentQueue;

		void CreateInstance();

		VkResult CreateVkIntanceApp(
			const VkInstanceCreateInfo *createInfo,
			const VkAllocationCallbacks *allocator,
			VkInstance *instance);

		bool CheckValidationLayerSupport();

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
			void *userData);

		void SetupDebugMessenger();

		VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
			const VkAllocationCallbacks *pAllocator,
			VkDebugUtilsMessengerEXT *pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks *pAllocator);

		void PopulateMessengerCreateInfo(
			VkDebugUtilsMessengerCreateInfoEXT &createInfo);

		// PhysicalDevice
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		// PhysicalDevice

		// LogicalDevice

		void CreateLogicalDevice();

		// LogicalDevice

		// void CreateSurface
		void CreateSurface();
		// void CreateSurface
	};
}