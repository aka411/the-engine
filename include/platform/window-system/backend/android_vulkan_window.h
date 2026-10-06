#pragma once
#include <platform/window-system/i_window.h>
#include <string_view>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>
#include <optional>

struct android_app;

namespace TheEngine::Platform
{

	class AndroidVulkanWindow final : public IWindow
	{
	private:

		android_app* m_androidApp{ nullptr };

	private:

		std::optional<RenderingSystem::VulkanBackend::VulkanRenderDevice> m_vulkanRenderDevice;

	public:
		AndroidVulkanWindow(const EngineConfiguration& engineConfiguration);

		virtual WindowExtent getWindowExtent() const override;
		virtual RenderingSystem::IRenderDevice& getRenderDevice() override;


		//
		void onWindowCreated();
		void onWindowDestroyed();



	};

}