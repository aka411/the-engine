
#include <rendering-system/api-backend/vulkan/vulkan_texture_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_transfer_manager.h>
#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_types.h>
#include <vk_mem_alloc.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>
#include <rendering-system/api-backend/vulkan/vulkan_sampler_manager.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

	VkImageAspectFlags getVkAspectFlags(ResourceFormat format)
	{
		switch (format)
		{
			// Depth Only
		case ResourceFormat::D16_UNORM:
		case ResourceFormat::D32_FLOAT:
			return VK_IMAGE_ASPECT_DEPTH_BIT;

			//Depth + Stencil
		case ResourceFormat::D24_S8_UINT:
		case ResourceFormat::D32_S8_FLOAT:
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

			// Everything else is Color
			// Includes UNORM, SRGB, FLOAT, and UINT formats
		case ResourceFormat::UNKNOWN:
		default:
			//assert(false && "Unknown ResourceFormat used to create texture ");
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}

	VkImageViewType getVkImageViewType(TextureType type)
	{
		switch (type) 
		{
		case TextureType::TEXTURE_1D:       return VK_IMAGE_VIEW_TYPE_1D;
		case TextureType::TEXTURE_2D:       return VK_IMAGE_VIEW_TYPE_2D;
		case TextureType::TEXTURE_3D:       return VK_IMAGE_VIEW_TYPE_3D;
		case TextureType::TEXTURE_CUBE_MAP: return VK_IMAGE_VIEW_TYPE_CUBE;
		case TextureType::TEXTURE_2D_ARRAY: return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			// case TextureType::UNKNOWN:          return VK_IMAGE_VIEW_TYPE_2D; // Default fallback
		default:
			assert(false && "Unknown TextureType used to create texture ");
			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}




	void VulkanTextureManager::setVulkanTransferManager(VulkanTransferManager& vulkanTransferManager)
	{
		assert(m_vulkanTransferManager == nullptr && "Error : Tried to reassign Vulkan Transfer Manager");
		//assert(vulkanTransferManager != nullptr);
		m_vulkanTransferManager = &vulkanTransferManager;
	}

	void VulkanTextureManager::updateBindlessSlot(const uint64_t slotIndex, VkSampler sampler, VkImageView imageView)
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


	size_t VulkanTextureManager::storeVulkanTexture(const VulkanTexture vulkanTexture)
	{
		const size_t index = m_vulkanTextures.size();
		m_vulkanTextures.push_back(vulkanTexture);

		return index;
	}

	VulkanTextureManager::VulkanTextureManager(VulkanContext& vulkanContext, VulkanSamplerManager& vulkanSamplerManager) :
		m_vulkanContext(vulkanContext),
		m_vulkanSamplerManager(vulkanSamplerManager)
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
		binding.descriptorCount = maxImages; 
		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

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

	VulkanTextureManager::~VulkanTextureManager()
	{
		//TODO : IMPLEMENT THIS
	}


	TextureHandle VulkanTextureManager::createTexture(const TextureCreateInfo& textureCreateInfo)
	{

		const TextureDescription& desc = textureCreateInfo.desc;


		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = (desc.type == TextureType::TEXTURE_3D) ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = desc.width;
		imageInfo.extent.height = desc.height;
		imageInfo.extent.depth = desc.depth;
		imageInfo.mipLevels = std::max(1u, desc.mipLevelCount);
		imageInfo.arrayLayers = std::max(1u, desc.arrayLayerCount);
		imageInfo.format = resourceFormatToVkFormat(desc.format);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = textureUsageFlagsToVkImageUsageFlags(desc.usageFlags);
		imageInfo.samples = getVkSampleCount(desc.sampleCount);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//POINT OF HEADACHE IN DESIGN
		imageInfo.pQueueFamilyIndices = nullptr;



		if (desc.type == TextureType::TEXTURE_CUBE_MAP)
		{
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}


		VkImage vkImage;
		VmaAllocation allocation;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO; // Let VMA decide the best location
		// If this is a texture we update often from CPU, use VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT


		VkResult result = vmaCreateImage(
			m_vulkanContext.vmaAllocator,
			&imageInfo,
			&allocInfo,
			&vkImage,
			&allocation,
			nullptr
		);

		if (result != VK_SUCCESS)
		{
			assert(false && "Failed to allocate memory for image");
		}



		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = vkImage;
		viewInfo.viewType = getVkImageViewType(desc.type);
		viewInfo.format = imageInfo.format;
		viewInfo.subresourceRange.aspectMask = getVkAspectFlags(desc.format);
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = desc.mipLevelCount;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = desc.arrayLayerCount;

		VkImageView imageView;
		vkCreateImageView(m_vulkanContext.vkDevice, &viewInfo, nullptr, &imageView);


		VkSampler vkSampler = m_vulkanSamplerManager.getOrCreateVkSampler(textureCreateInfo.samplerCreateInfo);




	

		VulkanTexture vulkanTexture;

		vulkanTexture.createinfo = textureCreateInfo;
		vulkanTexture.vkImage = vkImage;
		vulkanTexture.vkImageView = imageView;
		vulkanTexture.vmaAllocation = allocation;




		TextureHandle textureHandle;
		textureHandle.id = storeVulkanTexture(vulkanTexture);
	



		if (textureCreateInfo.makeBindless)
		{
			uint64_t bindlessHandle = textureHandle.id;

			updateBindlessSlot(textureHandle.id, vkSampler, imageView);

		}

		return textureHandle;
	}


	TextureHandle VulkanTextureManager::createTexture(const TextureCreateInfo& info, TheEngine::Memory::MemoryBlock&& initialData)
	{

		 const TextureHandle textureHandle = createTexture(info);
       
        if (initialData.getData() != nullptr)
        {
            ImageTransferRequest transferRequest(std::move(initialData), info.desc);
     
			VkImage vkImage = getVulkanTexture(textureHandle).vkImage;

			assert(m_vulkanTransferManager != nullptr);
            m_vulkanTransferManager->transferToImage(std::move(transferRequest), vkImage);
        }

        return textureHandle;
	}





	void VulkanTextureManager::destroyTexture(const TextureHandle& textureHandle)
	{
		const auto vulkanTexture = getVulkanTexture(textureHandle);

		vmaDestroyImage(
			m_vulkanContext.vmaAllocator,
			vulkanTexture.vkImage,
			vulkanTexture.vmaAllocation);

	}




	VkDescriptorSet VulkanTextureManager::getBindlessImageVkDescriptorSet()
	{

		return m_bindlessImageVkDescriptorSet;

	}

    VulkanTexture VulkanTextureManager::getVulkanTexture(const TextureHandle& textureHandle)
    {
		return m_vulkanTextures[textureHandle.id];
    }

}