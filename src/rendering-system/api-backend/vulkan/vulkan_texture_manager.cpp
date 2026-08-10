
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

	TextureHandle VulkanTextureManager::createTexture(const TextureCreateInfo& info)
	{
		const TextureHandle textureHandle = m_vulkanTextureStore.createNewTexture(info, m_textureRegistry);

		return textureHandle;
	}

	 TextureHandle VulkanTextureManager::createTexture(const TextureCreateInfo& info, TheEngine::Memory::MemoryBlock&& initialData)
	{

		 const TextureHandle textureHandle = createTexture(info);
       
        if (initialData.getData() != nullptr)
        {
            ImageTransferRequest transferRequest(std::move(initialData), info.desc);
     
			VkImage vkImage = m_vulkanTextureStore.getVulkanTexture(textureHandle).vkImage;
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