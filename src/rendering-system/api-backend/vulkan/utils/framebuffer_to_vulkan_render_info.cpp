#include <volk.h>
#include <rendering-system/api-backend/vulkan/utils/framebuffer_to_vulkan_render_info.h>
#include <rendering-system/rhi/data-structures/gpu_types.h>
#include <rendering-system/engine_handles.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_resource_resolver.h>
#include <rendering-system/rhi/framebuffer.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{


	static VkAttachmentLoadOp mapLoadOp(const AttachmentLoadOp op)
	{
		switch (op)
		{
		case AttachmentLoadOp::LOAD:      return VK_ATTACHMENT_LOAD_OP_LOAD;
		case AttachmentLoadOp::CLEAR:     return VK_ATTACHMENT_LOAD_OP_CLEAR;
		case AttachmentLoadOp::DONT_CARE:  return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		default:                          return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}
	}

	static VkAttachmentStoreOp mapStoreOp(const AttachmentStoreOp op)
	{
		switch (op)
		{
		case AttachmentStoreOp::STORE:     return VK_ATTACHMENT_STORE_OP_STORE;
		case AttachmentStoreOp::DONT_CARE: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		case AttachmentStoreOp::NONE:      return VK_ATTACHMENT_STORE_OP_NONE;
		default:                           return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
	}

	static VkResolveModeFlagBits mapToResolveMode(const AttachmentResolveMode resolveMode)
	{
		switch (resolveMode)
		{
		case AttachmentResolveMode::NONE:        return VK_RESOLVE_MODE_NONE;
		case AttachmentResolveMode::SAMPLE_ZERO: return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
		case AttachmentResolveMode::AVERAGE:     return VK_RESOLVE_MODE_AVERAGE_BIT;
		case AttachmentResolveMode::MIN:         return VK_RESOLVE_MODE_MIN_BIT;
		case AttachmentResolveMode::MAX:         return VK_RESOLVE_MODE_MAX_BIT;
		default:                                 return VK_RESOLVE_MODE_NONE;
		}
	}



	void toVulkanRenderingInfo(VulkanRenderingInfo& vulkanRenderingInfo, const RenderingSystem::Framebuffer& framebuffer,VulkanResourceResolver& resolver)//,VulkanTextureManager& vulkanTextureManager)
	{



		auto getVkImageView = [&resolver](const TextureHandle textureHandle) -> VkImageView
			{
				return resolver.getVulkanTexture(textureHandle).vkImageView;
			};


		vulkanRenderingInfo.colourAttachmentCount = framebuffer.getColorAttachmentCount();
		const auto& colorAttachments = framebuffer.getColorAttachments();
		if (!framebuffer.enabledMSAA())
		{
			for (int i = 0; i < vulkanRenderingInfo.colourAttachmentCount; ++i)
			{
				const auto& src = colorAttachments[i];
				VkRenderingAttachmentInfo& dst = vulkanRenderingInfo.vkColorRenderingAttachmentInfos[i];

				dst = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
				dst.imageView = getVkImageView(src.image);
				dst.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				dst.loadOp = mapLoadOp(src.imageAttachmentOps.load);
				dst.storeOp = mapStoreOp(src.imageAttachmentOps.store);


				dst.clearValue.color.float32[0] = src.clearValue.colour.x;
				dst.clearValue.color.float32[1] = src.clearValue.colour.y;
				dst.clearValue.color.float32[2] = src.clearValue.colour.z;
				dst.clearValue.color.float32[3] = src.clearValue.colour.w;
			}


			if (framebuffer.hasDepth())
			{
				const auto& src = framebuffer.getDepthAttachment();
				VkRenderingAttachmentInfo& dst = vulkanRenderingInfo.depthAttachment;

				dst = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
				dst.imageView = getVkImageView(src.image);
				dst.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
				dst.loadOp = mapLoadOp(src.imageAttachmentOps.load);
				dst.storeOp = mapStoreOp(src.imageAttachmentOps.store);
				dst.clearValue.depthStencil.depth = src.clearValue.depthStencil.depth;
			}


			if (framebuffer.hasStencil())
			{
				const auto& src = framebuffer.getStencilAttachment();
				VkRenderingAttachmentInfo& dst = vulkanRenderingInfo.stencilAttachment;

				dst = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
				dst.imageView = getVkImageView(src.image);
				dst.imageLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
				dst.loadOp = mapLoadOp(src.imageAttachmentOps.load);
				dst.storeOp = mapStoreOp(src.imageAttachmentOps.store);
				dst.clearValue.depthStencil.stencil = src.clearValue.depthStencil.stencil;
			}

		}
		else
		{

				for (int i = 0; i < vulkanRenderingInfo.colourAttachmentCount; ++i)
				{
					const auto& src = colorAttachments[i];
					VkRenderingAttachmentInfo& dst = vulkanRenderingInfo.vkColorRenderingAttachmentInfos[i];

					dst = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
					dst.imageView = getVkImageView(src.image);
					dst.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					dst.loadOp = mapLoadOp(src.imageAttachmentOps.load);
					dst.storeOp = mapStoreOp(src.imageAttachmentOps.store);

					dst.resolveImageView = getVkImageView(src.resolveImage);
					dst.resolveImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					dst.resolveMode = mapToResolveMode(src.resolveMode);
				

					dst.clearValue.color.float32[0] = src.clearValue.colour.x;
					dst.clearValue.color.float32[1] = src.clearValue.colour.y;
					dst.clearValue.color.float32[2] = src.clearValue.colour.z;
					dst.clearValue.color.float32[3] = src.clearValue.colour.w;
				}


				if (framebuffer.hasDepth())
				{
					const auto& src = framebuffer.getDepthAttachment();
					VkRenderingAttachmentInfo& dst = vulkanRenderingInfo.depthAttachment;

					dst = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };

					dst.imageView = getVkImageView(src.image);

					dst.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

					dst.loadOp = mapLoadOp(src.imageAttachmentOps.load);
					dst.storeOp = mapStoreOp(src.imageAttachmentOps.store);

					if (src.resolveImage.isValid())
					{
						dst.resolveImageView = getVkImageView(src.resolveImage);
						dst.resolveImageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
						dst.resolveMode = mapToResolveMode(src.resolveMode);
					}

					dst.clearValue.depthStencil.depth = src.clearValue.depthStencil.depth;




				}


				if (framebuffer.hasStencil())
				{
					const auto& src = framebuffer.getStencilAttachment();
					VkRenderingAttachmentInfo& dst = vulkanRenderingInfo.stencilAttachment;

					dst = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
					dst.imageView = getVkImageView(src.image);
					dst.imageLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
					dst.loadOp = mapLoadOp(src.imageAttachmentOps.load);
					dst.storeOp = mapStoreOp(src.imageAttachmentOps.store);


					if (src.resolveImage.isValid())
					{
						dst.resolveImageView = getVkImageView(src.resolveImage);
						dst.resolveImageLayout = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
						dst.resolveMode = mapToResolveMode(src.resolveMode);
					}


					dst.clearValue.depthStencil.stencil = src.clearValue.depthStencil.stencil;
				}




		}


		VkRenderingInfo& info = vulkanRenderingInfo.vkRenderingInfo;
		info = { VK_STRUCTURE_TYPE_RENDERING_INFO };

		
		info.renderArea = { {0, 0}, {framebuffer.getWidth(), framebuffer.getHeight()}};
		info.layerCount = 1;

		info.colorAttachmentCount = vulkanRenderingInfo.colourAttachmentCount;
		info.pColorAttachments = (info.colorAttachmentCount > 0) ? vulkanRenderingInfo.vkColorRenderingAttachmentInfos.data() : nullptr;

		info.pDepthAttachment = framebuffer.hasDepth() ? &vulkanRenderingInfo.depthAttachment : nullptr;
		info.pStencilAttachment = framebuffer.hasStencil() ? &vulkanRenderingInfo.stencilAttachment : nullptr;



	

	}




}