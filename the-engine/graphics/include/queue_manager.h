#pragma once

#include <vulkan_device.h>




class QueueManager
{
	//queue manager is not the owner of the queues, its vkbootstrap that creates the queues and has ownership of them
private:
	//Our graphics card does have spearate queues for 
	VkQueue m_graphicsQueueHandle = VK_NULL_HANDLE;
	VkQueue m_presentQueue = VK_NULL_HANDLE;


	int m_graphicsQueueFamilyIndex = -1;
	int m_presentQueueFamilyIndex = -1;




	bool m_graphicsQueueFamilyHasPresentationSupport = false;
	bool m_isUnifiedQueue = false;


	
public:
	QueueManager(VulkanDevice& vulkanDevice);

	VkQueue& getUnifiedQueueHandle() { return m_graphicsQueueHandle; }
	uint32_t getUnifiedQueueFamilyIndex() { return m_graphicsQueueFamilyIndex; }


	~QueueManager();


};