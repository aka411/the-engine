#pragma once

namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanFramebufferHelper
	{

	private:

		VulkanFramebufferHelper() = delete;
		~VulkanFramebufferhelper() = delete;  // do i need to delete this cause constructor is deleted?

	public:

		static VkRenderingInfoKHR createVkRenderingInfo(const Framebuffer& framebuffer);

	};


}