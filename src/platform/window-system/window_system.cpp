#include <platform/window-system/window_system.h>

#include <core/engine_core_data_types.h>
#include <assert.h>

#include <platform/window-system/i_window.h>


#ifdef PLATFORM_PC

#include <platform/window-system/backend/desktop_vulkan_window.h>

#elif PLATFORM_ANDROID

#include <platform/window-system/backend/android_vulkan_window.h>

#endif



namespace TheEngine::Platform
{


	WindowSystem::WindowSystem(const EngineConfiguration& engineConfiguration) 
	{


		

		switch (engineConfiguration.renderingAPI)
		{
		case RenderingAPI::OPENGL_4_6:
		{

			assert(false && "OPENGL DISABLED CURRENTLY");
			break;
		}

		case RenderingAPI::VULKAN_1_3:
		{
		
#ifdef PLATFORM_PC

			m_window = std::make_unique<DesktopVulkanWindow>(engineConfiguration);

#elif PLATFORM_ANDROID

			m_window = std::make_unique<AndroidVulkanWindow>(engineConfiguration);

#elif PLATFORM_TEST
			//Do nothing
#else
	#error "Neither PLATFORM_PC nor PLATFORM_ANDROID is defined!"
#endif

			break;
		}

		default:

			assert(false && "Unsupported Rendering API");
			break;
		}

	}

	WindowSystem::~WindowSystem()
	{

	}

	WindowExtent WindowSystem::getWindowExtent() const
	{
		return m_window->getWindowExtent();
	}


	IWindow& WindowSystem::getWindow()
	{
		return *m_window;
	}

	RenderingSystem::IRenderDevice& WindowSystem::getRenderDevice()
	{
		return m_window->getRenderDevice();
	}


}
