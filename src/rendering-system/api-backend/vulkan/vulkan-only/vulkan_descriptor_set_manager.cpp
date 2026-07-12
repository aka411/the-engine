#include <volk.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_descriptor_set_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

	static VkDescriptorType toVkType(ResourceType resourceType)
	{
		switch (resourceType)
		{
		case ResourceType::SAMPLER:                return VK_DESCRIPTOR_TYPE_SAMPLER;
		case ResourceType::COMBINED_IMAGE_SAMPLER: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case ResourceType::SAMPLED_IMAGE:          return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		case ResourceType::STORAGE_IMAGE:          return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		case ResourceType::UNIFORM_TEXEL_BUFFER:   return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		case ResourceType::STORAGE_TEXEL_BUFFER:   return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
		case ResourceType::UNIFORM_BUFFER:         return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case ResourceType::STORAGE_BUFFER:         return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case ResourceType::UNIFORM_BUFFER_DYNAMIC: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		case ResourceType::STORAGE_BUFFER_DYNAMIC: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		case ResourceType::INPUT_ATTACHMENT:       return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		case ResourceType::ACCELERATION_STRUCTURE: return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
		default:                                   return VK_DESCRIPTOR_TYPE_MAX_ENUM;
		}
	}

	static VkShaderStageFlags toVkShaderStageFlags(ShaderStageFlags flags)
	{
		VkShaderStageFlags vkFlags = 0;
		uint32_t f = static_cast<uint32_t>(flags);

		if (f & static_cast<uint32_t>(ShaderStageFlags::VERTEX))                  vkFlags |= VK_SHADER_STAGE_VERTEX_BIT;
		if (f & static_cast<uint32_t>(ShaderStageFlags::FRAGMENT))                vkFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (f & static_cast<uint32_t>(ShaderStageFlags::COMPUTE))                 vkFlags |= VK_SHADER_STAGE_COMPUTE_BIT;
		if (f & static_cast<uint32_t>(ShaderStageFlags::GEOMETRY))                vkFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
		if (f & static_cast<uint32_t>(ShaderStageFlags::TESSELLATION_CONTROL))    vkFlags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (f & static_cast<uint32_t>(ShaderStageFlags::TESSELLATION_EVALUATION)) vkFlags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		return vkFlags;
	}


	VkDescriptorSetLayout VulkanDescriptorSetManager::createVkDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout)
	{
		
		std::vector<VkDescriptorSetLayoutBinding> vkDescriptorSetLayoutBindings;
		vkDescriptorSetLayoutBindings.reserve(descriptorSetLayout.getBindings().size());

		for (auto& bind : descriptorSetLayout.getBindings())
		{
			VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding{};
			vkDescriptorSetLayoutBinding.binding = bind.binding;
			vkDescriptorSetLayoutBinding.descriptorType = toVkType(bind.type);
			
			vkDescriptorSetLayoutBinding.descriptorCount = bind.count;
			
			vkDescriptorSetLayoutBinding.stageFlags = toVkShaderStageFlags(bind.stageFlags);
	
			vkDescriptorSetLayoutBindings.push_back(vkDescriptorSetLayoutBinding);

		}





		VkDescriptorSetLayout vkDescriptorSetLayout;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = vkDescriptorSetLayoutBindings.size();
		layoutInfo.pBindings = vkDescriptorSetLayoutBindings.data();

		



		VkDescriptorBindingFlags flags = 0;
		VkDescriptorSetLayoutBindingFlagsCreateInfo layoutFlags{};
		if (descriptorSetLayout.isBindless)
		{
			assert(descriptorSetLayout.getBindings().size() == 1 && "Shader contract violated : Bindless Descriptor set contains more than 1 binding");
			flags =
				VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT |
				VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
				VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;
			
			vkDescriptorSetLayoutBindings[0].descriptorCount = 4096;//TODO : FIX THIS HARD CODING , BINDLESS TEXTURE MANAGER AND THIS MUST MATCH

			layoutFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
			layoutFlags.bindingCount =1;
			layoutFlags.pBindingFlags = &flags;
			layoutInfo.pNext = &layoutFlags;
			layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
		}


		if (vkCreateDescriptorSetLayout(m_vulkanContext.vkDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS)
		{
			assert(false && "Failed to create descriptor set layout!");
		}


		return vkDescriptorSetLayout;




	}


	void VulkanDescriptorSetManager::createVkDescriptorPool(VkDescriptorPoolCreateInfo& vkDescriptorPoolCreateInfo, VkDescriptorPool& vkDescriptorPool)
	{

		if (vkCreateDescriptorPool(m_vulkanContext.vkDevice, &vkDescriptorPoolCreateInfo, nullptr, &vkDescriptorPool) != VK_SUCCESS)
		{
			assert(false && "Failed to create DescriptorSetPool");
		}
	}




	void VulkanDescriptorSetManager::resetDynamicPool(const uint32_t frameIndex)
	{
		assert(frameIndex < m_perFrameVkDescriptorPool.size());


		vkResetDescriptorPool(m_vulkanContext.vkDevice, m_perFrameVkDescriptorPool[frameIndex], 0);


	}



	VulkanDescriptorSetManager::VulkanDescriptorSetManager(VulkanContext& vulkanContext) :
		m_vulkanContext(vulkanContext)
	{




		std::array<VkDescriptorPoolSize, 5> staticPoolSize{};

		// Sampled Images (Textures)
		staticPoolSize[0].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		staticPoolSize[0].descriptorCount = 4096;

		// Uniform Buffer Objects (UBO) 
		staticPoolSize[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		staticPoolSize[1].descriptorCount = 2048;

		// Shader Storage Buffer Objects (SSBO)
		staticPoolSize[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		staticPoolSize[2].descriptorCount = 2048;

		// Combined Image Samplers 
		staticPoolSize[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		staticPoolSize[3].descriptorCount = 4096;

		//Storage Images 
		staticPoolSize[4].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		staticPoolSize[4].descriptorCount = 1024;




		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.pNext = nullptr;
		descriptorPoolCreateInfo.flags = 0;// VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.maxSets = 8192;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(staticPoolSize.size());
		descriptorPoolCreateInfo.pPoolSizes = staticPoolSize.data();

		






		createVkDescriptorPool(descriptorPoolCreateInfo, m_staticVkDescriptorPool);


		VkDescriptorPoolCreateInfo dynamicDescriptorPoolCreateInfo{};
		dynamicDescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		dynamicDescriptorPoolCreateInfo.pNext = nullptr;
		dynamicDescriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		dynamicDescriptorPoolCreateInfo.maxSets = 8192;
		dynamicDescriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(staticPoolSize.size());
		dynamicDescriptorPoolCreateInfo.pPoolSizes = staticPoolSize.data();





		for (uint32_t i = 0; i < NUMBER_OF_FRAMES_IN_FLIGHT; i++) 
		{
			createVkDescriptorPool(dynamicDescriptorPoolCreateInfo, m_perFrameVkDescriptorPool[i]);
		}


	}











	VkDescriptorSetLayout VulkanDescriptorSetManager::getOrCreateVkDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout)
	{

		//first check if its already there

		const auto& hash = descriptorSetLayout.getHash();

		if (m_cachedVkDescriptorSetLayouts.find(hash) == m_cachedVkDescriptorSetLayouts.end())
		{
			//None exists
			m_cachedVkDescriptorSetLayouts.insert({ hash,createVkDescriptorSetLayout(descriptorSetLayout)});

		}


		return m_cachedVkDescriptorSetLayouts.at(hash);
	}




	
	VkDescriptorSet VulkanDescriptorSetManager::createVkDescriptorSet(const VkDescriptorSetLayout& vDescriptorSetLayout, DescriptorPoolType descriptorPoolType)
	{

		//TODO : STORE STATIC POOL ALLOCATED DESCRIPTOR SET HERE TO BE FREED LATER

	


		VkDescriptorPool descriptorPool;


		switch (descriptorPoolType)
		{
		case DescriptorPoolType::STATIC_POOL :
		{
			descriptorPool = m_staticVkDescriptorPool;
			break;
		}
		case DescriptorPoolType::DYNAMIC_POOL :
		{
			descriptorPool = m_perFrameVkDescriptorPool[m_currentFrameIndex];
			break;
		}

		default:
		{
			descriptorPool = m_perFrameVkDescriptorPool[m_currentFrameIndex];
			break;
		}
		}



		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

		allocInfo.descriptorPool = descriptorPool;

		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &vDescriptorSetLayout;




		VkDescriptorSet vkDescriptorSet;



		if (vkAllocateDescriptorSets(m_vulkanContext.vkDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS)
		{
			assert(false && "Failed to allocate descriptor set!");
		}



		return vkDescriptorSet;

	}




	void VulkanDescriptorSetManager::setCurrentFrame(const uint32_t frameIndex)
	{

		assert(frameIndex < NUMBER_OF_FRAMES_IN_FLIGHT);

		m_currentFrameIndex = frameIndex;

		resetDynamicPool(frameIndex);
	}


}