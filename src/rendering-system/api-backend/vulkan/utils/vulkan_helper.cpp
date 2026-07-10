#include <rendering-system/api-backend/vulkan/utils/vulkan_helper.h>
#include <stdexcept>

namespace TheEngine::RenderingSystem::VulkanBackend
{




	VkFence createVkFence(VkDevice& vkDevice, bool signaled)
	{



		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		VkFence vkFence;

		if (vkCreateFence(vkDevice, &fenceInfo, nullptr, &vkFence) != VK_SUCCESS)
		{

			throw std::runtime_error("failed to create fence!");
		}


		return vkFence;
	}

	VkSemaphore createVkSemaphore(VkDevice& vkDevice)
	{

		
	VkSemaphoreCreateInfo semaphoreCreateinfo{};
	semaphoreCreateinfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateinfo.pNext = nullptr;
	semaphoreCreateinfo.flags = 0;

	VkSemaphore vkSemaphore;

	if (vkCreateSemaphore(vkDevice, &semaphoreCreateinfo, nullptr, &vkSemaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create semaphores!");
	}

	return vkSemaphore;
}






}