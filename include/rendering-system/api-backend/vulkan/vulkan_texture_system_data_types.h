#pragma once
#include <volk.h>
#include <vk_mem_alloc.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{



	struct VulkanTexture
	{
		TextureCreateInfo createinfo{};

		VkImage vkImage;
		VkImageView vkImageView;
		VmaAllocation vmaAllocation;

	};

	struct VulkanImageViewCombined
	{
		VkImage image = VK_NULL_HANDLE;
		VkImageView imageView = VK_NULL_HANDLE;
		//VkFormat imageFormat = VK_FORMAT_UNDEFINED;
	};




}