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

			if (!layerFound) return false;
		}

		return true;
	}

	std::vector<const char*> VulkanInstance::GetRequiredExtensions()
	{
		uint32_t glfwExtensionsCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

		if (_enableValidationLayers) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData
	)
	{
		std::cerr << "Validations layerid: " << callbackData->pMessageIdName << " layers message: " << callbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	void VulkanInstance::SetupDebugMessenger()
	{
		if (!_enableValidationLayers) return;
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

	VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const
		VkDebugUtilsMessengerCreateInfoEXT* createInfo, const
		VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT*
		debugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
			vkGetInstanceProcAddr(instance,
				"vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, createInfo, allocator, debugMessenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger, const
		VkAllocationCallbacks* allocator) {
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

		if (_physicalDevice == VK_NULL_HANDLE) throw std::runtime_error("Failed to find physical device suitable supported");
	}

	bool VulkanInstance::IsDeviceSuitable(VkPhysicalDevice physicalDevice)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);



		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
	}

	uint32_t VulkanInstance::FindQueueFamilies(VkPhysicalDevice physicalDevice)
	{

	}

}