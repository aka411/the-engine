#include "descriptor_set_manager.h"
#include "volk.h"
DescriptorSetManager::DescriptorSetManager(VulkanDevice& vulkanDevice) : m_vulkanDevice(vulkanDevice)
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



	if (vkCreateDescriptorPool(vulkanDevice.getLogicalDeviceHandle(), &descriptorPoolCreateInfo, nullptr, &m_globalImageDescriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create DescriptorSetPool");
	}


	/*
	
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 4096;
	allocInfo.pSetLayouts = layouts.data();
	*/
	/*
	VkDescriptorSetLayoutBinding globalImageArrayLayoutBinding{};
	globalImageArrayLayoutBinding.binding = 0;
	globalImageArrayLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	globalImageArrayLayoutBinding.descriptorCount = 1;

	globalImageArrayLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	globalImageArrayLayoutBinding.pImmutableSamplers = nullptr; // Optional
	globalImageArrayLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;//VK_ALL_GRAPHICS

	*/
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 0;
	layoutInfo.pBindings = nullptr;// &globalImageArrayLayoutBinding;

	if (vkCreateDescriptorSetLayout(m_vulkanDevice.getLogicalDeviceHandle(), &layoutInfo, nullptr, &m_globalImageArrayDescriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}


	m_descriptorSetlayoutList.push_back(m_globalImageArrayDescriptorSetLayout);



	//TODO : WRITE CODE FOR m_globalCameraAndLightSSBODescriptorSetLayout similar to one above





}




std::vector<VkDescriptorSetLayout>& DescriptorSetManager::getDescriptorSetLayout()
{
	return m_descriptorSetlayoutList;

}