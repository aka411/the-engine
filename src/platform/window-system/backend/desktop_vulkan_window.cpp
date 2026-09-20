#include <platform/window-system/backend/desktop_vulkan_window.h>


#include <volk.h>
#include <VkBootstrap.h>

#include <platform/window-system/window_system.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>


#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>

namespace TheEngine::Platform
{



	DesktopVulkanWindow::DesktopVulkanWindow(const EngineConfiguration& engineConfiguration)
	{



		SDL_Init(SDL_INIT_VIDEO);
		m_sdlWindow = SDL_CreateWindow(
			"TheEngine(Vulkan 1.3)",
			engineConfiguration.logicalWindowExtend.width, engineConfiguration.logicalWindowExtend.height,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
		);


		uint32_t extensionCount{ 0 };
		const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

		volkInitialize();

		vkb::InstanceBuilder builder;
		builder.set_app_name("Vulkan Renderer")
			.request_validation_layers(true)
			.require_api_version(1, 3, 0)
			.use_default_debug_messenger()
			.enable_extensions(extensionCount, extensions);

		auto instRet = builder.build();

		if (!instRet)
		{
			std::cout << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
			assert(false && "Failed to create Vulkan instance");
		}

		vkb::Instance vkbInstance = instRet.value();


		volkLoadInstanceOnly(vkbInstance.instance);

		VkSurfaceKHR vkSurfaceKHR;

		if (!SDL_Vulkan_CreateSurface(m_sdlWindow, vkbInstance.instance, nullptr, &vkSurfaceKHR))
		{

			std::cout << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
			assert(false && "Failed to create Vulkan instance");
		}


		m_vulkanRenderDevice.emplace(vkSurfaceKHR, vkbInstance);
	}




	WindowExtent DesktopVulkanWindow::getWindowExtent() const
	{
		WindowExtent windowExtent{};
		auto& logical = windowExtent.logical;
		auto& framebuffer = windowExtent.framebuffer;

		assert(m_sdlWindow != nullptr && "SDL Window is nullptr");

		int logicalWidth{ 0 };
		int logicalHeight{ 0 };

		int framebufferWidth{ 0 };
		int framebufferHeight{ 0 };

		if (!SDL_GetWindowSize(m_sdlWindow, &logicalWidth, &logicalHeight))
		{
			assert(false && "Failed to get logical window size using SDL");
		}

		if (!SDL_GetWindowSizeInPixels(m_sdlWindow, &framebufferWidth, &framebufferHeight))
		{
			assert(false && "Failed to get framebuffer size using SDL");
		}


		logical.width = logicalWidth;
		logical.height = logicalHeight;
		framebuffer.width = framebufferWidth;
		framebuffer.height = framebufferHeight;

		return windowExtent;
	}


	RenderingSystem::IRenderDevice& DesktopVulkanWindow::getRenderDevice()
	{
		assert(m_vulkanRenderDevice.has_value() && " ");
		return *m_vulkanRenderDevice;
	}

	//SDL_DestroyWindow(m_sdlWindow);
	//SDL_Quit();
}