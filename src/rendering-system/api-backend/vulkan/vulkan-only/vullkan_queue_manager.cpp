#include <rendering-system/api-backend/vulkan/vulkan-only/vullkan_queue_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>

#include <VkBootstrap.h>//Order of Include matters , there is a conflict with volk and this

namespace TheEngine::RenderingSystem::VulkanBackend
{


	static bool hasCapability(const TheEngine::RenderingSystem::VulkanBackend::QueueType type,
		const TheEngine::RenderingSystem::VulkanBackend::QueueType typeToBeCheckedAgainst)
	{

		if (type & typeToBeCheckedAgainst)
		{
			return true;
		}
		return false;
	};





	VkQueueFlags VulkanQueueManager::toVkQueueFlags(const TheEngine::RenderingSystem::VulkanBackend::QueueType type)
	{

		VkQueueFlags flags = 0;

		if (hasCapability(type, QueueType::GRAPHICS)) flags |= VK_QUEUE_GRAPHICS_BIT;
		if (hasCapability(type, QueueType::COMPUTE))  flags |= VK_QUEUE_COMPUTE_BIT;
		if (hasCapability(type, QueueType::TRANSFER)) flags |= VK_QUEUE_TRANSFER_BIT;

		return flags;
	}


	VulkanQueueManager::VulkanQueueManager(VulkanContext& vulkanContext) :
		m_vulkanContext(vulkanContext)
	{


	}

	VulkanQueueManager::~VulkanQueueManager()
	{


	}


	QueueAddress VulkanQueueManager::getQueueFamilyAddress(const TheEngine::RenderingSystem::VulkanBackend::QueueType queueType) const
	{


		QueueAddress queueAddress;


		for (uint32_t i = 0; i < m_vulkanContext.vkbDevice.queue_families.size(); i++)
		{
			auto& queueFamily = m_vulkanContext.vkbDevice.queue_families[i];

			if (queueFamily.queueFlags & toVkQueueFlags(queueType))
			{
				//we dont need queueFamily.queueFlags here but just kept there 
				if (queueFamily.queueFlags & toVkQueueFlags(TheEngine::RenderingSystem::VulkanBackend::QueueType::GRAPHICS) & toVkQueueFlags(queueType))
				{
					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(m_vulkanContext.vkPhysicalDevice, i, m_vulkanContext.vkSurfaceKHR, &presentSupport);
					if (!presentSupport)
					{
						continue;
					}
				}

				queueAddress.queueFamilyIndex = i;

				vkGetDeviceQueue(m_vulkanContext.vkDevice, queueAddress.queueFamilyIndex, 0, &queueAddress.vkQueue);
			
				return queueAddress;
			}

		}


		assert(false && "Requested queue type not found on this physical device.");
		return queueAddress;

	}

}
