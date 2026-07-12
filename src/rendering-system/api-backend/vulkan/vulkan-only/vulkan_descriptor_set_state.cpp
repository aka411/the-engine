#include <assert.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_descriptor_set_state.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_descriptor_set_manager.h>




namespace TheEngine::RenderingSystem::VulkanBackend
{




	VulkanDescriptorSetState::VulkanDescriptorSetState()
	{

	}

	VulkanDescriptorSetState::VulkanDescriptorSetState(VkCommandBuffer& vKCommandBuffer, VkPipelineLayout vkPipelineLayout,VkDescriptorSet globalBindlessTexturevkDescriptorSet, std::span<VkDescriptorSetLayout> vkDescriptorSetLayouts, VulkanDescriptorSetManager& vulkanDescriptorSetManager)
	{

		m_VkDescriptorSets.reserve(vkDescriptorSetLayouts.size());

		m_VkDescriptorSets.push_back(globalBindlessTexturevkDescriptorSet);

		for (int i = 1; i < vkDescriptorSetLayouts.size(); ++i)
		{


			m_VkDescriptorSets.push_back(vulkanDescriptorSetManager.createVkDescriptorSet(vkDescriptorSetLayouts[i], DescriptorPoolType::DYNAMIC_POOL));

		}

	
			 
		
			vkCmdBindDescriptorSets(
				vKCommandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				vkPipelineLayout,
				0,                          // Starting at set 0
				m_VkDescriptorSets.size(),                         
				m_VkDescriptorSets.data(),
				0, nullptr                  // No dynamic offsets
			);
		

		
	}

	void VulkanDescriptorSetState::bindBuffer(uint32_t set, uint32_t binding, VkDescriptorBufferInfo info, VkDescriptorType type)
	{
		auto& pWrite = m_VkDescriptorSetWrites[set][binding];
		pWrite.info.bufferInfo = info;
		pWrite.dirty = true;

		pWrite.vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		pWrite.vkWriteDescriptorSet.dstSet = m_VkDescriptorSets[set];
		pWrite.vkWriteDescriptorSet.dstBinding = binding;
		pWrite.vkWriteDescriptorSet.descriptorCount = 1;

		pWrite.vkWriteDescriptorSet.descriptorType = type;

		// pWrite.write.pBufferInfo will be set during the 'Flush' phase



	}





	void VulkanDescriptorSetState::flushUpdates(VkDevice device)
	{
		std::vector<VkWriteDescriptorSet> writesToSubmit;

		for (uint32_t s = 0; s < 4; ++s)
		{
			for (auto& entry : m_VkDescriptorSetWrites[s])
			{
				if (entry.dirty)
				{


					if (entry.vkWriteDescriptorSet.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
						entry.vkWriteDescriptorSet.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
					{
						entry.vkWriteDescriptorSet.pBufferInfo = &entry.info.bufferInfo;
					}
					else 
					{
						entry.vkWriteDescriptorSet.pImageInfo = &entry.info.imageInfo;
					}

					writesToSubmit.push_back(entry.vkWriteDescriptorSet);
					entry.dirty = false;
				}
			}
		}

		if (!writesToSubmit.empty())
		{
			vkUpdateDescriptorSets(device, static_cast<uint32_t>(writesToSubmit.size()),
				writesToSubmit.data(), 0, nullptr);
		}
	}









	VulkanDescriptorSetState::~VulkanDescriptorSetState()
	{
		for (uint32_t s = 0; s < 4; ++s)
		{
			for (auto& entry : m_VkDescriptorSetWrites[s])
			{
				if (entry.dirty)
				{
					assert(false && "Unused VkDescriptorSetWrites");
				}
			}
		}
	}




}