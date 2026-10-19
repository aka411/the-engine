#pragma once
#include <map>

#include <volk.h>
#include <rendering-system/rhi/i_command_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_command_buffer.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vullkan_queue_manager.h>






namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanContext;
	class VulkanDescriptorSetManager;
	//class VulkanQueueManager;

	class VulkanCommandBufferManager //: ICommandBufferManager
	{

	private:

		VulkanContext& m_vulkanContext;
		VulkanQueueManager& m_vulkanQueueManager;

		std::map<QueueType, VkCommandPool > m_commandPools;

		VulkanResourceResolver& m_vulkanResourceResolver;
		VulkanDescriptorSetManager& m_vulkanDescriptorSetManager;

	public:

		VulkanCommandBufferManager(VulkanContext& vulkanContext, VulkanQueueManager& vulkanQueueManager, VulkanResourceResolver& vulkanResourceResolver, VulkanDescriptorSetManager& vulkanDescriptorSetManager);


		VulkanCommandBuffer getCommandBuffer(TheEngine::RenderingSystem::VulkanBackend::QueueType queueType);


	};









}