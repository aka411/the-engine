#include <rendering-system/api-backend/vulkan/vulkan_command_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>




namespace TheEngine::RenderingSystem::VulkanBackend
{





	VulkanCommandBufferManager::VulkanCommandBufferManager(VulkanContext& vulkanContext, VulkanQueueManager& vulkanQueueManager, VulkanResourceResolver& vulkanResourceResolver, VulkanDescriptorSetManager& vulkanDescriptorSetManager) :
		m_vulkanContext(vulkanContext),
		m_vulkanQueueManager(vulkanQueueManager),
		m_vulkanResourceResolver(vulkanResourceResolver),
		m_vulkanDescriptorSetManager(vulkanDescriptorSetManager)
	{
	


	}



	VulkanCommandBuffer VulkanCommandBufferManager::getCommandBuffer(TheEngine::RenderingSystem::VulkanBackend::QueueType queueType)
	{
		//uint32_t familyIndex = getQueueFamilyIndex(queueType);
		QueueAddress queueAddress = m_vulkanQueueManager.getQueueFamilyAddress(queueType);
		if (m_commandPools.find(queueType) == m_commandPools.end())
		{
		

			VkCommandPoolCreateInfo commandPoolInfo = {};
			commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			commandPoolInfo.queueFamilyIndex = queueAddress.queueFamilyIndex;
			commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //allow command buffers to be individually reset
			commandPoolInfo.pNext = nullptr;

			VkCommandPool vkCommandPool;

			if ((vkCreateCommandPool(m_vulkanContext.vkDevice, &commandPoolInfo, nullptr, &vkCommandPool) != VK_SUCCESS))
			{
				assert(false && "Failed to create command pool");
			}


			m_commandPools[queueType] = vkCommandPool;


		}


		VkCommandPool& vkCommandPool = m_commandPools.at(queueType);
		

	
		VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.commandBufferCount = 1; 
		commandBufferAllocInfo.commandPool = vkCommandPool;
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 
		commandBufferAllocInfo.pNext = nullptr;



		VkCommandBuffer vkCommandBuffer;


		if ((vkAllocateCommandBuffers(m_vulkanContext.vkDevice, &commandBufferAllocInfo, &vkCommandBuffer) != VK_SUCCESS))
		{
			assert(false && "Failed to allocate command buffers");
		}




		VulkanCommandBuffer vulkanCommandBuffer(m_vulkanContext,vkCommandBuffer, queueAddress.queueFamilyIndex, queueAddress.vkQueue, m_vulkanResourceResolver, m_vulkanDescriptorSetManager);


		return vulkanCommandBuffer;



	}










}