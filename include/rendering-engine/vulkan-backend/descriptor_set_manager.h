#pragma once
#include <vulkan_device.h>

//A toplevel manager
class DescriptorSetManager
{


private:

	VulkanDevice& m_vulkanDevice;


	VkDescriptorPool m_globalImageDescriptorPool;//not used for mvp

	VkDescriptorPool m_globalCameraAndLightSSBOPool;//not used for mvp

	VkDescriptorSetLayout m_globalImageArrayDescriptorSetLayout;//not used for mvp

	VkDescriptorSetLayout m_cameraAndLightSSBODescriptorSetLayout;//not used for mvp


	std::vector<VkDescriptorSetLayout> m_descriptorSetlayoutList;

public:

	DescriptorSetManager(VulkanDevice& vulkanDevice);
	std::vector<VkDescriptorSetLayout>& getDescriptorSetLayout();

};