#include <platform/window-system/backend/android_vulkan_window.h>


#include <volk.h>
#include <VkBootstrap.h>

#include <platform/window-system/window_system.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>


#include <game-activity/native_app_glue/android_native_app_glue.h>


namespace TheEngine::Platform
{

	AndroidVulkanWindow::AndroidVulkanWindow(const EngineConfiguration& engineConfiguration)
	{


		m_androidApp = engineConfiguration.androidApp;

		assert(m_androidApp != nullptr && "VulkanWindow : m_androidApp is nullptr");
		assert(m_androidApp->window != nullptr);

		std::vector<const char*> required_instance_extensions{ VK_KHR_SURFACE_EXTENSION_NAME };
		required_instance_extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);


		volkInitialize();
		uint32_t extensionCount{ 2 };
		vkb::InstanceBuilder builder;
		builder.set_app_name("Vulkan Renderer")
			.request_validation_layers(false)
			.require_api_version(1, 3, 0)
			//.use_default_debug_messenger(false)
			.enable_extensions(extensionCount, required_instance_extensions.data());

		auto instRet = builder.build();

		if (!instRet)
		{
			//std::cout << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
			assert(false && "Failed to create Vulkan instance");
		}

		vkb::Instance vkbInstance = instRet.value();


		volkLoadInstanceOnly(vkbInstance.instance);

		VkSurfaceKHR vkSurfaceKHR;
		assert(m_androidApp->window != nullptr && "ANativeWindow is nullptr");
		VkAndroidSurfaceCreateInfoKHR vkAndroidSurfaceCreateInfoKHR{};
		vkAndroidSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
		vkAndroidSurfaceCreateInfoKHR.flags = 0;
		vkAndroidSurfaceCreateInfoKHR.window = m_androidApp->window;
		vkAndroidSurfaceCreateInfoKHR.pNext = nullptr;

		//const auto vkInstance = m_vulkanRenderDevice->getInstance();
		if (vkCreateAndroidSurfaceKHR == nullptr) {
			assert(false && "Failed to create android surface");
			// Handle error
		}
		const auto surfaceCreationResult = vkCreateAndroidSurfaceKHR(vkbInstance.instance, &vkAndroidSurfaceCreateInfoKHR, nullptr, &vkSurfaceKHR);

		if (surfaceCreationResult != VK_SUCCESS)
		{
			assert(false && "Failed to create android surface");
		}



		m_vulkanRenderDevice.emplace(vkSurfaceKHR, vkbInstance);

	}

	WindowExtent AndroidVulkanWindow::getWindowExtent() const
	{

		WindowExtent windowExtent{};
		auto& logical = windowExtent.logical;
		auto& framebuffer = windowExtent.framebuffer;

		int logicalWidth{ 0 };
		int logicalHeight{ 0 };

		int framebufferWidth{ 0 };
		int framebufferHeight{ 0 };

		assert(m_androidApp != nullptr && "m_androidApp is nullptr");
		auto* androidNativeWindow = m_androidApp->window;
		assert(androidNativeWindow != nullptr && " androidNativeWindow is nullptr");

		logical.width = AConfiguration_getScreenWidthDp(m_androidApp->config);
		logical.height = AConfiguration_getScreenHeightDp(m_androidApp->config);
		framebuffer.width = ANativeWindow_getWidth(androidNativeWindow);
		framebuffer.height = ANativeWindow_getHeight(androidNativeWindow);

		return windowExtent;
	}

	RenderingSystem::IRenderDevice& AndroidVulkanWindow::getRenderDevice()
	{
		assert(m_vulkanRenderDevice.has_value() && " ");
		return *m_vulkanRenderDevice;
	}


	void AndroidVulkanWindow::onWindowCreated()
	{
		assert(m_androidApp->window != nullptr);

		VkSurfaceKHR vkSurfaceKHR{};

		VkAndroidSurfaceCreateInfoKHR vkAndroidSurfaceCreateInfoKHR{};
		vkAndroidSurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
		vkAndroidSurfaceCreateInfoKHR.flags = 0;
		vkAndroidSurfaceCreateInfoKHR.window = m_androidApp->window;
		vkAndroidSurfaceCreateInfoKHR.pNext = nullptr;

		const auto vkInstance = m_vulkanRenderDevice->getInstance();

		vkCreateAndroidSurfaceKHR(vkInstance, &vkAndroidSurfaceCreateInfoKHR, nullptr, &vkSurfaceKHR);


		m_vulkanRenderDevice->setVkSurfaceKHR(vkSurfaceKHR);
	}



	void AndroidVulkanWindow::onWindowDestroyed()
	{
		assert(false && "Implement AndroidVulkanWindow::onWindowDestroyed() ");
	}

}