#pragma once

namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanDescriptorSetManager : public IDescriptorSetManager
	{

	private:
		
		VkDescriptorPool m_descriptorPool;

	public:
		VulkanDescriptorSetManager();


	};




}