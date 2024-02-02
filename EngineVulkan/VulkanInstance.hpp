#pragma once
#define GLFW_INCLUDE_VULKAN
#include "EngineVulkanTypes.hpp"
#include <algorithm>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include "Utils.hpp"

namespace EngineVulkan
{
	class VulkanInstance
	{
	public:
		const std::vector<const char*> _validationLayers = {
			"VK_LAYER_KHRONOS_validation" };

		const std::vector<const char*> _deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		std::vector<VkImage> _swapChainImages;
		std::vector<VkImageView> _swapChainImageViews;

		std::vector<VkDynamicState> _dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

#if NDEBUG
		const bool _enableValidationLayers = false;
#else
		const bool _enableValidationLayers = true;
#endif

		VkDebugUtilsMessengerEXT _debugMessenger = nullptr;

		void Initialize(int width, int height, std::string windowName);
		void InitializeWindow(int width, int height, std::string windowName);
		void InitializeVulkan();

		GLFWwindow* GetWindow() const { return _window; }
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

		void DestroySwapChainApp()
		{
			vkDestroySwapchainKHR(_device, _swapChain, nullptr);
		}

		void DestroyImagesViewsApp()
		{
			for (auto imageView : _swapChainImageViews)
				vkDestroyImageView(_device, imageView, nullptr);
		}

	private:
		GLFWwindow* _window = nullptr;
		std::string _nameWindow;
		int _width;
		int _height;

		std::vector<const char*> GetRequiredExtensions() const;

		VkInstance _instance;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDevice _device;

		VkSurfaceKHR _surface;
		VkQueue _graphicsQueue;
		VkQueue _presentQueue;
		VkSwapchainKHR _swapChain;

		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;

		void CreateInstance();

		VkResult CreateVkIntanceApp(
			const VkInstanceCreateInfo* createInfo,
			const VkAllocationCallbacks* allocator,
			VkInstance* instance);

		bool CheckValidationLayerSupport();

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* userData);

		void SetupDebugMessenger();

		VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);

		void PopulateMessengerCreateInfo(
			VkDebugUtilsMessengerCreateInfoEXT& createInfo);

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

		// SwapChain
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateSwapChain();
		// SwapChain

		// ImageViews
		void CreateImageViews();
		// ImageViews

		// GraphicsPipeline
		void CreateGraphicsPipeline(std::string fileVertex, std::string fileFragment);
		// GraphicsPipeline

		// ShaderModule
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		// ShaderModule
	};
}