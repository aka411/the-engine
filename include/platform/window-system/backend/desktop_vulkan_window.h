#pragma once
#include <platform/window-system/i_window.h>
#include <string_view>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>
#include <optional>


struct SDL_Window;


namespace TheEngine::Platform
{

	class DesktopVulkanWindow final : public IWindow
	{

	private:

		SDL_Window* m_sdlWindow{ nullptr };

	private:

		std::optional<RenderingSystem::VulkanBackend::VulkanRenderDevice> m_vulkanRenderDevice;

	public:

		DesktopVulkanWindow(const EngineConfiguration& engineConfiguration);

		virtual WindowExtent getWindowExtent() const override;
		virtual RenderingSystem::IRenderDevice& getRenderDevice() override;

	};



}