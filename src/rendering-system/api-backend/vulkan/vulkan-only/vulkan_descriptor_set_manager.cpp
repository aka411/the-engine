

namespace TheEngine::RenderingSystem::VulkanBackend
{



	VulkanDescriptorSetManager::VulkanDescriptorSetManager
	{

	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	poolSize.descriptorCount = 4096; // its an array




	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = nullptr;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = 4096;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &poolSize;



	if (vkCreateDescriptorPool(vulkanDevice.getLogicalDeviceHandle(), &descriptorPoolCreateInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create DescriptorSetPool");
	}




	}


}