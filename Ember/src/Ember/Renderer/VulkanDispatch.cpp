// Minimal TU owning Vulkan-Hpp dynamic dispatcher storage for static nvrhi_vk.
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include "Ember/Renderer/VulkanDispatch.h"

namespace Ember {

	void InitVulkanDispatch(VkInstance instance, VkDevice device)
	{
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vk::Instance(instance), vk::Device(device));
	}

}
