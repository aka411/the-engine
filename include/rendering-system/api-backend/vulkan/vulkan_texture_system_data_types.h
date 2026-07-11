#pragma once
#include <volk.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{




	struct VulkanImageViewCombined
	{
		VkImage image = VK_NULL_HANDLE;
		VkImageView imageView = VK_NULL_HANDLE;
		//VkFormat imageFormat = VK_FORMAT_UNDEFINED;
	};




}