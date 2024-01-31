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
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
	}

	void VulkanInstance::CreateInstance()
	{
		if (_enableValidationLayers && !CheckValidationLayerSupport())
			throw std::runtime_error("Not support validation layers or not availables layers support");

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_3;
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pApplicationName = "Vulkan Example Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "EngineVulkanExample";

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (_enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();
			PopulateMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (CreateVkIntanceApp(&createInfo, nullptr, &_instance) != VK_SUCCESS)
			throw std::runtime_error("Create instance vulkan error");
	}

	VkResult VulkanInstance::CreateVkIntanceApp(
		const VkInstanceCreateInfo* createInfo,
		const VkAllocationCallbacks* allocator,
		VkInstance* instance)
	{
		if (createInfo == nullptr || instance == nullptr)
			return VK_ERROR_INITIALIZATION_FAILED;

		return vkCreateInstance(createInfo, allocator, instance);
	}

	bool VulkanInstance::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto& layerName : _validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperty : availableLayers)
			{
				if (strcmp(layerName, layerProperty.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	std::vector<const char*> VulkanInstance::GetRequiredExtensions() const
	{
		uint32_t glfwExtensionsCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

		if (_enableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

#if __APPLE__ || __MACH__
		extensions.push_back("VK_KHR_get_physical_device_properties2");
#endif

		return extensions;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData)
	{
		std::cerr << "Validations layerid: " << callbackData->pMessageIdName << " layers message: " << callbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	void VulkanInstance::SetupDebugMessenger()
	{
		if (!_enableValidationLayers)
			return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		PopulateMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
			throw std::runtime_error("Failed to set up debug message info");
	}

	void VulkanInstance::PopulateMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr; // Optional
	}

	VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(instance,
				"vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, createInfo, allocator, debugMessenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(instance, debugMessenger, allocator);
	}

	void VulkanInstance::PickPhysicalDevice()
	{
		std::map<int, VkPhysicalDevice> candidates;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			throw std::runtime_error("Failed to find GPU's with vulkan support");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				_physicalDevice = device;
				break;
			}
		}

		if (_physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to find physical device suitable supported");
	}

	bool VulkanInstance::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		bool swapChainAdequate = false;
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.IsComplete() && extensionsSupported && swapChainAdequate;
	}

	bool VulkanInstance::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanInstance::FindQueueFamilies(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices{};
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, _surface, &presentSupport);

			if (presentSupport)
				indices.presentFamily = i;
			if (indices.IsComplete())
				break;

			i++;
		}

		return indices;
	}

	void VulkanInstance::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		VkDeviceCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = _deviceExtensions.data();

		if (_enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
			createInfo.ppEnabledLayerNames = _validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device ");

		vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
		vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
	}

	SwapChainSupportDetails VulkanInstance::QuerySwapChainSupport(VkPhysicalDevice physicalDevice)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, _surface, &details.capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, details.formats.data());

		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	void VulkanInstance::CreateSurface()
	{
		if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface!");
	}

	VkSurfaceFormatKHR VulkanInstance::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanInstance::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanInstance::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width !=
			std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;

		}
		else {
			int width, height;
			glfwGetFramebufferSize(_window, &width, &height);

			VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width,
				capabilities.minImageExtent.width,
				capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height,
				capabilities.minImageExtent.height,
				capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}