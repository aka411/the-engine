#include <rendering-system/engine_handles.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_texture_store.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>
#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_types.h>
#include <rendering-system/api-backend/vulkan/vulkan_sampler_manager.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{


    TextureHandle VulkanTextureStore::storeVulkanTexture(VulkanTexture& vulkanTexture, std::vector<TextureMetadata>& textureRegistry)
    {

        TextureHandle textureHandle;
        textureHandle.id = m_vulkanTextures.size();
        m_vulkanTextures.push_back(vulkanTexture);
        textureRegistry.push_back(vulkanTexture.metaData);
        return  textureHandle;

    }


    VulkanTextureStore::VulkanTextureStore(VulkanContext& vulkanContext, VulkanSamplerManager& vulkanSamplerManager):
        m_vulkanContext(vulkanContext),
        m_vulkanSamplerManager(vulkanSamplerManager),
        m_vulkanBindlessTextureSetManager(vulkanContext)
    {

        m_vulkanTextures.push_back(VulkanTexture{});
        
    }


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

    VkImageViewType getVkImageViewType(TextureType type) {
        switch (type) {
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



    TextureHandle VulkanTextureStore::createNewTexture(TextureCreateInfo& textureCreateInfo, std::vector<TextureMetadata>& textureRegistry)
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

        //Create Image View
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = vkImage;
        viewInfo.viewType = getVkImageViewType(desc.type); 
        viewInfo.format = imageInfo.format;
        viewInfo.subresourceRange.aspectMask = getVkAspectFlags(desc.format); // Logic needed here for Depth formats
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = desc.mipLevelCount;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = desc.arrayLayerCount;

        VkImageView imageView;
        vkCreateImageView(m_vulkanContext.vkDevice, &viewInfo, nullptr, &imageView);


        VkSampler vkSampler = m_vulkanSamplerManager.getOrCreateVkSampler(textureCreateInfo.samplerCreateInfo);




        TextureMetadata metadata;

        metadata.desc = textureCreateInfo.desc;
        metadata.currentLayout = textureCreateInfo.initialLayout;


   

        //store metadata
        VulkanTexture vulkanTexture;

        vulkanTexture.metaData = metadata;
        vulkanTexture.vkImage = vkImage;
        vulkanTexture.vkImageView = imageView;
        vulkanTexture.vmaAllocation = allocation;

        TextureHandle textureHandle = storeVulkanTexture(vulkanTexture, textureRegistry);



        if (textureCreateInfo.makeBindless)
        {
            uint64_t bindlessHandle = textureHandle.id;

            m_vulkanBindlessTextureSetManager.updateBindlessSlot(textureHandle.id, vkSampler, imageView);

            m_vulkanTextures[textureHandle.id].metaData.bindlessHandle = textureHandle.id;//THIS IS BAD DESIGN I NEED TO CHANGE LATER
            m_vulkanTextures[textureHandle.id].metaData.isBindless = textureCreateInfo.makeBindless;
            m_vulkanTextures[textureHandle.id].metaData.bindlessHandle = bindlessHandle;



            textureRegistry[textureHandle.id].bindlessHandle = textureHandle.id;//THIS IS BAD DESIGN I NEED TO CHANGE LATER
            textureRegistry[textureHandle.id].isBindless = textureCreateInfo.makeBindless;
            textureRegistry[textureHandle.id].bindlessHandle = bindlessHandle;
        }


        return textureHandle;
    }

    void VulkanTextureStore::destroyTexture(const TextureHandle& textureHandle, std::vector<TextureMetadata>& textureRegistry)
    {

    }



    VkDescriptorSet VulkanTextureStore::getBindlessImageVkDescriptorSet()
    {
        return m_vulkanBindlessTextureSetManager.getBindlessImageVkDescriptorSet();
    }


    VulkanTexture VulkanTextureStore::getVulkanTexture(const TextureHandle textureHandle)
    {

            assert(m_vulkanTextures.size() > textureHandle.id && "Invalid TextureHandle used to get VulkanTexture");
            return m_vulkanTextures[textureHandle.id];

    }



}