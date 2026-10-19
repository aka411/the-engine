#pragma once
#include <volk.h>
#include <array>

namespace TheEngine::RenderingSystem
{
	class Framebuffer;
}

namespace TheEngine::RenderingSystem::VulkanBackend
{



	//class VulkanTextureManager;
	class VulkanResourceResolver;


	struct VulkanRenderingInfo
	{

		std::array<VkRenderingAttachmentInfo, 8> vkColorRenderingAttachmentInfos{};

		VkRenderingAttachmentInfo depthAttachment{};
		VkRenderingAttachmentInfo stencilAttachment{};

		VkRenderingInfo vkRenderingInfo{};
		int colourAttachmentCount = 0;

	};

	void toVulkanRenderingInfo(VulkanRenderingInfo& vulkanRenderingInfo, const RenderingSystem::Framebuffer& framebuffer, VulkanResourceResolver& resolver);// VulkanTextureManager& vulkanTextureManager);





}