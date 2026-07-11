#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_bindless_texture_set_manager.h>
#include <assert.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{


		VulkanBindlessTextureSetManager::VulkanBindlessTextureSetManager(VulkanContext& vulkanContext) :
			m_vulkanContext(vulkanContext)
		{



			//This has to match when creating pipeline aslo
			uint32_t maxImages = 4096;

			VkDescriptorPoolSize imagePoolSizes[] = {
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, maxImages }
			};

			VkDescriptorPoolCreateInfo imagePoolInfo{};
			imagePoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			imagePoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
			imagePoolInfo.maxSets = 1; // We only need one giant set
			imagePoolInfo.poolSizeCount = 1;
			imagePoolInfo.pPoolSizes = imagePoolSizes;


			if (vkCreateDescriptorPool(m_vulkanContext.vkDevice, &imagePoolInfo, nullptr, &m_bindlessImageVkDescriptorPool) != VK_SUCCESS)
			{
				assert(false && "Failed to create bindless descriptor pool!");
			}



			VkDescriptorSetLayoutBinding binding{};
			binding.binding = 0;
			binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding.descriptorCount = maxImages; //
			binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Usually fragment, or ALL

			VkDescriptorBindingFlags flags =
				VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT |
				VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
				VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;

			VkDescriptorSetLayoutBindingFlagsCreateInfo layoutFlags{};
			layoutFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
			layoutFlags.bindingCount = 1;
			layoutFlags.pBindingFlags = &flags;

			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = &layoutFlags;
			layoutInfo.bindingCount = 1;
			layoutInfo.pBindings = &binding;
			layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;


			VkDescriptorSetLayout vkDescriptorSetLayout;

			vkCreateDescriptorSetLayout(m_vulkanContext.vkDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout);



			VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo{};
			variableCountInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
			variableCountInfo.descriptorSetCount = 1;
			variableCountInfo.pDescriptorCounts = &maxImages;

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.pNext = &variableCountInfo;
			allocInfo.descriptorPool = m_bindlessImageVkDescriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &vkDescriptorSetLayout;


			vkAllocateDescriptorSets(m_vulkanContext.vkDevice, &allocInfo, &m_bindlessImageVkDescriptorSet);





		}







		VkDescriptorSet VulkanBindlessTextureSetManager::getBindlessImageVkDescriptorSet()
		{

			return m_bindlessImageVkDescriptorSet;

		}





		void VulkanBindlessTextureSetManager::updateBindlessSlot(const uint64_t slotIndex, VkSampler sampler, VkImageView imageView)
		{


			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = imageView;
			imageInfo.sampler = sampler;

			VkWriteDescriptorSet write{};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = m_bindlessImageVkDescriptorSet;
			write.dstBinding = 0;
			write.dstArrayElement = slotIndex; 
			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.descriptorCount = 1;
			write.pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(m_vulkanContext.vkDevice, 1, &write, 0, nullptr);


		}



}