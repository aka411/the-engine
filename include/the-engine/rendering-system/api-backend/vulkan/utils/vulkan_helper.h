#pragma once
#include <volk.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{


	VkFence createVkFence(VkDevice& vkDevice, bool signaled = true);
	VkSemaphore createVkSemaphore(VkDevice& vkDevice);



}