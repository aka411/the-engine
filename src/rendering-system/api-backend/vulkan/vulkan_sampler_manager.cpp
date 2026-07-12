#include <rendering-system/api-backend/vulkan/vulkan_sampler_manager.h>
#include <rendering-system/rhi/data-structures/gpu_sampler_data_structures.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{


	VulkanSamplerManager::VulkanSamplerManager(VulkanContext& vulkanContext) :
		m_vulkanContext(vulkanContext)
	{

	}

	VulkanSamplerManager::~VulkanSamplerManager()
	{

	}

	const SamplerHandle VulkanSamplerManager::getOrCreateSampler(const TheEngine::RenderingSystem::SamplerCreateInfo& samplerCreateInfo)
	{


		
		SamplerHandle samplerHandle;
		assert(false && "Vulkan Sampler Manager Not yet complete");

		return SamplerHandle{};
	}




	VkSampler VulkanSamplerManager::getOrCreateVkSampler(const TheEngine::RenderingSystem::SamplerCreateInfo& samplerCreateInfo)
	{

		assert(false && "Vulkan Sampler Manager Not yet complete");

		//This class is hardcoded for MVP

		SamplerHandle samplerHandle;

		if (m_cachedVulkanSamplers.find(samplerCreateInfo) == m_cachedVulkanSamplers.end())
		{
			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

			// Filtering settings
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;

			// Wrapping (Address mode)
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

			// Anisotropic Filtering (Optional)
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = 16.0f; // Check limits via VkPhysicalDeviceProperties

			// Border and LOD
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE; // Use [0, 1] range
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = VK_LOD_CLAMP_NONE;

	
			VkSampler textureSampler;
			if (vkCreateSampler(m_vulkanContext.vkDevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create texture sampler!");
			}


			m_cachedVulkanSamplers[samplerCreateInfo] = textureSampler;
		}




		return m_cachedVulkanSamplers.at(samplerCreateInfo);
	}



	void VulkanSamplerManager::destroySampler(const SamplerHandle& samplerHandle)
	{

	}
}