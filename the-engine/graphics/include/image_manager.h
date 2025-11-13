#pragma once
#include "vk_mem_alloc.h"
#include <vulkan_device.h>



struct ImageAllocationInfo
{
	VkImage imageHandle = VK_NULL_HANDLE;
	VmaAllocation vmaAllocation = VK_NULL_HANDLE;

	size_t imageSize = 0;

	VkImageView imageView = VK_NULL_HANDLE;
	VkFormat imageFormat = VK_FORMAT_UNDEFINED;
	VkImageLayout imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	uint32_t width = 0;
	uint32_t height = 0;

};



class ImageManager
{
private:


public:

	ImageManager(VulkanDevice& vulkanDevice);
	
};