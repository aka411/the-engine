
#include <rendering-system/api-backend/vulkan/vulkan_texture_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_transfer_manager.h>
#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_types.h>




namespace TheEngine::RenderingSystem::VulkanBackend
{




	VulkanTextureManager::VulkanTextureManager(VulkanTextureStore& vulkanTextureStore, VulkanTransferManager& vulkanTransferManager) :
		m_vulkanTextureStore(vulkanTextureStore),
		m_vulkanTransferManager(vulkanTransferManager)
	{






	}

	VulkanTextureManager::~VulkanTextureManager()
	{
		//TODO : IMPLEMENT THIS
	}

    TextureHandle VulkanTextureManager::createNewTexture(TextureCreateInfo& textureCreateInfo)
	{


		const TextureHandle textureHandle = m_vulkanTextureStore.createNewTexture(textureCreateInfo,m_textureRegistry);

		VkImage vkImage = m_vulkanTextureStore.getVulkanTexture(textureHandle).vkImage;

       
        if (textureCreateInfo.memoryBlock.getData() != nullptr)
        {
            ImageTransferRequest transferRequest(std::move(textureCreateInfo.memoryBlock), textureCreateInfo.desc);
     

            this->m_vulkanTransferManager.transferToImage(std::move(transferRequest), vkImage);
        }

   

        return textureHandle;

	}

	void VulkanTextureManager::destroyTexture(const TextureHandle& textureHandle)
	{


		//TODO : IMPLEMENT THIS


	}




	VkDescriptorSet VulkanTextureManager::getBindlessImageVkDescriptorSet()
	{

		return m_vulkanTextureStore.getBindlessImageVkDescriptorSet();

	}

    VulkanTexture VulkanTextureManager::getVulkanTexture(const TextureHandle& textureHandle)
    {
		return m_vulkanTextureStore.getVulkanTexture(textureHandle);
    }


}