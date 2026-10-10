#pragma once

#include "Ember/Core/Base.h"

#include <nvrhi/nvrhi.h>
#include <nvrhi/vulkan.h>

#include <vector>
#include <functional>

struct GLFWwindow;

namespace Ember {

	class GraphicsDevice
	{
	public:
		GraphicsDevice() = default;
		~GraphicsDevice();

		bool Init(GLFWwindow* window, bool enableValidation = true);
		void Shutdown();

		void BeginFrame();
		void EndFrame();
		void Present();
		void ResizeSwapchain(uint32_t width, uint32_t height);

		nvrhi::IDevice* GetDevice() const { return m_Device; }
		nvrhi::ICommandList* GetCommandList() const { return m_CommandList; }
		nvrhi::IFramebuffer* GetCurrentFramebuffer() const;
		nvrhi::TextureHandle GetCurrentBackBuffer() const;

		uint32_t GetSwapchainWidth() const { return m_SwapchainWidth; }
		uint32_t GetSwapchainHeight() const { return m_SwapchainHeight; }
		uint32_t GetSwapchainIndex() const { return m_SwapchainIndex; }

		VkInstance GetVkInstance() const { return m_Instance; }
		VkDevice GetVkDevice() const { return m_VkDevice; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return m_PhysicalDevice; }
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		uint32_t GetGraphicsQueueFamily() const { return m_GraphicsQueueFamily; }

		static GraphicsDevice& Get() { return *s_Instance; }
		static bool IsInitialized() { return s_Instance != nullptr; }

	private:
		bool CreateInstance(bool enableValidation);
		bool CreateSurface(GLFWwindow* window);
		bool PickPhysicalDevice();
		bool CreateDevice();
		bool CreateSwapchain();
		void DestroySwapchain();

	private:
		static GraphicsDevice* s_Instance;

		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_VkDevice = VK_NULL_HANDLE;
		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;
		uint32_t m_GraphicsQueueFamily = 0;
		uint32_t m_PresentQueueFamily = 0;

		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		VkFormat m_SwapchainFormat = VK_FORMAT_B8G8R8A8_UNORM;
		VkExtent2D m_SwapchainExtent{};
		uint32_t m_SwapchainWidth = 0;
		uint32_t m_SwapchainHeight = 0;
		uint32_t m_SwapchainIndex = 0;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<nvrhi::TextureHandle> m_SwapchainTextures;
		std::vector<nvrhi::FramebufferHandle> m_SwapchainFramebuffers;

		static constexpr uint32_t MaxFramesInFlight = 2;
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		uint32_t m_CurrentFrame = 0;

		nvrhi::vulkan::DeviceHandle m_Device;
		nvrhi::CommandListHandle m_CommandList;

		bool m_ValidationEnabled = false;
		GLFWwindow* m_Window = nullptr;
	};

}
