#include "queue_manager.h"
#include <utlis.h>

QueueManager::QueueManager(VulkanDevice& vulkanDevice)
{
	vkb::Device vkb_device = vulkanDevice.getVKBLogicalDevice();




	auto graphicsRet = vkb_device.get_queue(vkb::QueueType::graphics);

	auto graphicsIndexRet = vkb_device.get_queue_index(vkb::QueueType::graphics);
	auto presentIndexRet = vkb_device.get_queue_index(vkb::QueueType::present);




	if (graphicsRet.has_value() && graphicsIndexRet.has_value())
	{

		m_graphicsQueueHandle = graphicsRet.value();
		m_graphicsQueueFamilyIndex = graphicsIndexRet.value();
	
		m_isUnifiedQueue = true;

		if (presentIndexRet.has_value())
		{
			m_presentQueueFamilyIndex = presentIndexRet.value();

			if (m_presentQueueFamilyIndex == m_graphicsQueueFamilyIndex)
			{
				m_graphicsQueueFamilyHasPresentationSupport = true;
			}
		}
		else
		{
			throw std::runtime_error("Failed to get Present Queue Family Index.");
		}
	}
	else
	{
		throw std::runtime_error("Failed to get Graphics Queue.");
	}





}

QueueManager::~QueueManager()
{


}
