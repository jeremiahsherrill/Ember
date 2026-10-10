#include "empch.h"
#include "Ember/Renderer/GraphicsDevice.h"
#include "Ember/Renderer/VulkanDispatch.h"

#include <GLFW/glfw3.h>

#include <set>
#include <cstring>
#include <array>

namespace Ember {

	GraphicsDevice* GraphicsDevice::s_Instance = nullptr;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		(void)messageType;
		(void)pUserData;

		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			EM_CORE_ERROR("Vulkan: {0}", pCallbackData->pMessage);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			EM_CORE_WARN("Vulkan: {0}", pCallbackData->pMessage);
		else
			EM_CORE_TRACE("Vulkan: {0}", pCallbackData->pMessage);

		return VK_FALSE;
	}

	GraphicsDevice::~GraphicsDevice()
	{
		Shutdown();
	}

	bool GraphicsDevice::Init(GLFWwindow* window, bool enableValidation)
	{
		EM_CORE_ASSERT(!s_Instance, "GraphicsDevice already initialized!");
		s_Instance = this;
		m_Window = window;
		m_ValidationEnabled = enableValidation;

		if (!CreateInstance(enableValidation))
			return false;
		if (!CreateSurface(window))
			return false;
		if (!PickPhysicalDevice())
			return false;
		if (!CreateDevice())
			return false;
		if (!CreateSwapchain())
			return false;

		m_CommandList = m_Device->createCommandList();
		EM_CORE_INFO("GraphicsDevice initialized (Vulkan / nvrhi)");
		return true;
	}

	void GraphicsDevice::Shutdown()
	{
		if (!s_Instance)
			return;

		if (m_VkDevice)
			vkDeviceWaitIdle(m_VkDevice);

		m_CommandList = nullptr;
		DestroySwapchain();

		m_Device = nullptr;

		if (m_VkDevice)
		{
			vkDestroyDevice(m_VkDevice, nullptr);
			m_VkDevice = VK_NULL_HANDLE;
		}

		if (m_Surface)
		{
			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
			m_Surface = VK_NULL_HANDLE;
		}

		if (m_DebugMessenger)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func)
				func(m_Instance, m_DebugMessenger, nullptr);
			m_DebugMessenger = VK_NULL_HANDLE;
		}

		if (m_Instance)
		{
			vkDestroyInstance(m_Instance, nullptr);
			m_Instance = VK_NULL_HANDLE;
		}

		s_Instance = nullptr;
		EM_CORE_INFO("GraphicsDevice shut down");
	}

	bool GraphicsDevice::CreateInstance(bool enableValidation)
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Ember";
		appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		appInfo.pEngineName = "Ember";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidation)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = (uint32_t)extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidation)
		{
			createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();

			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = DebugCallback;
			createInfo.pNext = &debugCreateInfo;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		if (result != VK_SUCCESS)
		{
			// Retry without validation if layers unavailable
			if (enableValidation)
			{
				EM_CORE_WARN("Validation layers unavailable, retrying without them");
				m_ValidationEnabled = false;
				createInfo.enabledLayerCount = 0;
				createInfo.ppEnabledLayerNames = nullptr;
				createInfo.pNext = nullptr;
				extensions.pop_back();
				createInfo.enabledExtensionCount = (uint32_t)extensions.size();
				result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
			}
			if (result != VK_SUCCESS)
			{
				EM_CORE_ERROR("Failed to create Vulkan instance: {0}", (int)result);
				return false;
			}
		}

		if (m_ValidationEnabled)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
			if (func)
			{
				VkDebugUtilsMessengerCreateInfoEXT info = debugCreateInfo;
				info.pNext = nullptr;
				func(m_Instance, &info, nullptr, &m_DebugMessenger);
			}
		}

		return true;
	}

	bool GraphicsDevice::CreateSurface(GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface) != VK_SUCCESS)
		{
			EM_CORE_ERROR("Failed to create window surface");
			return false;
		}
		return true;
	}

	bool GraphicsDevice::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			EM_CORE_ERROR("No Vulkan-capable GPUs found");
			return false;
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(device, &props);

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int graphicsFamily = -1;
			int presentFamily = -1;
			for (uint32_t i = 0; i < queueFamilyCount; i++)
			{
				if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					graphicsFamily = (int)i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
				if (presentSupport)
					presentFamily = (int)i;

				if (graphicsFamily >= 0 && presentFamily >= 0)
					break;
			}

			if (graphicsFamily < 0 || presentFamily < 0)
				continue;

			uint32_t extensionCount = 0;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> available(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, available.data());

			bool hasSwapchain = false;
			for (const auto& ext : available)
			{
				if (strcmp(ext.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
					hasSwapchain = true;
			}
			if (!hasSwapchain)
				continue;

			m_PhysicalDevice = device;
			m_GraphicsQueueFamily = (uint32_t)graphicsFamily;
			m_PresentQueueFamily = (uint32_t)presentFamily;
			EM_CORE_INFO("Selected GPU: {0}", props.deviceName);
			return true;
		}

		EM_CORE_ERROR("Failed to find a suitable GPU");
		return false;
	}

	bool GraphicsDevice::CreateDevice()
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { m_GraphicsQueueFamily, m_PresentQueueFamily };

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
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_VkDevice) != VK_SUCCESS)
		{
			EM_CORE_ERROR("Failed to create logical device");
			return false;
		}

		vkGetDeviceQueue(m_VkDevice, m_GraphicsQueueFamily, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_VkDevice, m_PresentQueueFamily, 0, &m_PresentQueue);

		InitVulkanDispatch(m_Instance, m_VkDevice);

		nvrhi::vulkan::DeviceDesc deviceDesc;
		deviceDesc.errorCB = nullptr;
		deviceDesc.instance = m_Instance;
		deviceDesc.physicalDevice = m_PhysicalDevice;
		deviceDesc.device = m_VkDevice;
		deviceDesc.graphicsQueue = m_GraphicsQueue;
		deviceDesc.graphicsQueueIndex = (int)m_GraphicsQueueFamily;
		deviceDesc.allocationCallbacks = nullptr;

		m_Device = nvrhi::vulkan::createDevice(deviceDesc);
		if (!m_Device)
		{
			EM_CORE_ERROR("Failed to create nvrhi device");
			return false;
		}

		return true;
	}

	bool GraphicsDevice::CreateSwapchain()
	{
		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, formats.data());

		VkSurfaceFormatKHR chosenFormat = formats[0];
		for (const auto& format : formats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				chosenFormat = format;
				break;
			}
		}
		m_SwapchainFormat = chosenFormat.format;

		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);
		VkExtent2D extent = {
			std::clamp((uint32_t)width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			std::clamp((uint32_t)height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};
		if (capabilities.currentExtent.width != UINT32_MAX)
			extent = capabilities.currentExtent;

		m_SwapchainExtent = extent;
		m_SwapchainWidth = extent.width;
		m_SwapchainHeight = extent.height;

		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
			imageCount = capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = chosenFormat.format;
		createInfo.imageColorSpace = chosenFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		uint32_t queueFamilyIndices[] = { m_GraphicsQueueFamily, m_PresentQueueFamily };
		if (m_GraphicsQueueFamily != m_PresentQueueFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		createInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(m_VkDevice, &createInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
		{
			EM_CORE_ERROR("Failed to create swapchain");
			return false;
		}

		vkGetSwapchainImagesKHR(m_VkDevice, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_VkDevice, m_Swapchain, &imageCount, m_SwapchainImages.data());

		m_SwapchainTextures.resize(imageCount);
		m_SwapchainFramebuffers.resize(imageCount);

		for (uint32_t i = 0; i < imageCount; i++)
		{
			nvrhi::TextureDesc textureDesc;
			textureDesc.width = m_SwapchainWidth;
			textureDesc.height = m_SwapchainHeight;
			textureDesc.format = nvrhi::Format::BGRA8_UNORM;
			textureDesc.debugName = "SwapchainImage";
			textureDesc.isRenderTarget = true;
			textureDesc.initialState = nvrhi::ResourceStates::Present;
			textureDesc.keepInitialState = true;

			m_SwapchainTextures[i] = m_Device->createHandleForNativeTexture(
				nvrhi::ObjectTypes::VK_Image,
				nvrhi::Object(m_SwapchainImages[i]),
				textureDesc);

			nvrhi::FramebufferDesc fbDesc;
			fbDesc.addColorAttachment(m_SwapchainTextures[i]);
			m_SwapchainFramebuffers[i] = m_Device->createFramebuffer(fbDesc);
		}

		m_ImageAvailableSemaphores.resize(MaxFramesInFlight);
		m_RenderFinishedSemaphores.resize(MaxFramesInFlight);
		m_InFlightFences.resize(MaxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32_t i = 0; i < MaxFramesInFlight; i++)
		{
			vkCreateSemaphore(m_VkDevice, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);
			vkCreateSemaphore(m_VkDevice, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);
			vkCreateFence(m_VkDevice, &fenceInfo, nullptr, &m_InFlightFences[i]);
		}

		return true;
	}

	void GraphicsDevice::DestroySwapchain()
	{
		for (auto& fence : m_InFlightFences)
			if (fence) vkDestroyFence(m_VkDevice, fence, nullptr);
		for (auto& sem : m_ImageAvailableSemaphores)
			if (sem) vkDestroySemaphore(m_VkDevice, sem, nullptr);
		for (auto& sem : m_RenderFinishedSemaphores)
			if (sem) vkDestroySemaphore(m_VkDevice, sem, nullptr);

		m_InFlightFences.clear();
		m_ImageAvailableSemaphores.clear();
		m_RenderFinishedSemaphores.clear();

		m_SwapchainFramebuffers.clear();
		m_SwapchainTextures.clear();
		m_SwapchainImages.clear();

		if (m_Swapchain)
		{
			vkDestroySwapchainKHR(m_VkDevice, m_Swapchain, nullptr);
			m_Swapchain = VK_NULL_HANDLE;
		}
	}

	void GraphicsDevice::ResizeSwapchain(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		vkDeviceWaitIdle(m_VkDevice);
		DestroySwapchain();
		CreateSwapchain();
	}

	void GraphicsDevice::BeginFrame()
	{
		vkWaitForFences(m_VkDevice, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(m_VkDevice, m_Swapchain, UINT64_MAX,
			m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &m_SwapchainIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			int width, height;
			glfwGetFramebufferSize(m_Window, &width, &height);
			ResizeSwapchain(width, height);
			return;
		}

		vkResetFences(m_VkDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		m_CommandList->open();
	}

	void GraphicsDevice::EndFrame()
	{
		m_CommandList->close();
		m_Device->executeCommandList(m_CommandList);
	}

	void GraphicsDevice::Present()
	{
		// Signal via nvrhi queue submit already happened; present with semaphores
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 0;
		presentInfo.pWaitSemaphores = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &m_SwapchainIndex;

		VkResult result = vkQueuePresentKHR(m_PresentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			int width, height;
			glfwGetFramebufferSize(m_Window, &width, &height);
			ResizeSwapchain(width, height);
		}

		m_Device->runGarbageCollection();
		m_CurrentFrame = (m_CurrentFrame + 1) % MaxFramesInFlight;
	}

	nvrhi::IFramebuffer* GraphicsDevice::GetCurrentFramebuffer() const
	{
		return m_SwapchainFramebuffers[m_SwapchainIndex];
	}

	nvrhi::TextureHandle GraphicsDevice::GetCurrentBackBuffer() const
	{
		return m_SwapchainTextures[m_SwapchainIndex];
	}

}
