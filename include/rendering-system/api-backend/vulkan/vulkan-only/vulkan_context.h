#pragma once
#include <volk.h>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{


	struct VulkanContext
	{
		VkDevice vkDevice;
		VkPhysicalDevice vkPhysicalDevice;
		VkInstance vkInstance;
		VkSurfaceKHR vkSurfaceKHR;

		VmaAllocator vmaAllocator;
		vkb::Device vkbDevice;
	};

}