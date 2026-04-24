#include <SDL3/SDL_init.h>
#include <platform/window_system.h>

//#include <rendering-system/api-backend/opengl/utils/opengl_buffer_helper.h>
//#include <glad.h>
//#include <rendering-system/api-backend/opengl/opengl_render_device.h>	

#include <cassert>
#include <iostream>
#include <engine/engine_core_data_types.h>

#include <volk.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>


namespace TheEngine::Platform
{









	WindowSystem::WindowSystem(const EngineConfiguration& engineConfiguration) 
	{

		SDL_Init(SDL_INIT_VIDEO);

		

		switch (engineConfiguration.renderingAPI)
		{
		case RenderingAPI::OPENGL_4_6:
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

			m_window = SDL_CreateWindow(
				"TheEngine(openGL 4.6)",
				engineConfiguration.windowWidth, engineConfiguration.windowHeight,
				SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
			);



			SDL_GLContext  gl_context = SDL_GL_CreateContext(m_window);

			SDL_GL_MakeCurrent(m_window, gl_context);
	
			/*
			gladLoadGL();
			gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

			

			//yeah whos gonna delete this? whats its lifetime
			const GLubyte* rawString = glGetString(GL_RENDERER);

			if (rawString != nullptr)
			{
				m_gpuVendor = reinterpret_cast<const char*>(rawString);
			}

			m_renderDevice = std::make_unique<TheEngine::RenderingSystem::OpenGLBackend::OpenglRenderDevice>();	

			*/
			assert(false && "OPENGL DISABLED CURRENTLY");
			break;
		}

		case RenderingAPI::VULKAN_1_3://why not vulkan 1.4 --> I need to later downlaod latest drivers to support vulkan 1.4
		{
		

			m_window = SDL_CreateWindow(
				"TheEngine(Vulkan 1.3)",
				engineConfiguration.windowWidth, engineConfiguration.windowHeight,
				SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
			);


			//pass the SDL_VK_GetProcAddress to volk

			//volkInitializeCustom((PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr);

			volkInitialize();//from sdk




			uint32_t extensionCount = 0;
			const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

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

			vkb::Instance instance = instRet.value();






			volkLoadInstanceOnly(instance.instance);

			VkSurfaceKHR surface;

			if (!SDL_Vulkan_CreateSurface(m_window, instance.instance, nullptr, &surface))
			{
			
				std::cout << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
				assert(false && "Failed to create Vulkan instance");
			}




			m_renderDevice = std::make_unique<TheEngine::RenderingSystem::VulkanBackend::VulkanRenderDevice>(surface, instance);



			break;
		}

		default:
			assert(false && "Unsupported Rendering API");
			break;
		}

	}

	WindowSystem::~WindowSystem()
	{

	
		//SDL_GL_DeleteContext(m_iRenderingAPIContext.get()-;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	std::unique_ptr<TheEngine::RenderingSystem::IRenderDevice> WindowSystem::getRenderDevice()
	{
		return std::move(m_renderDevice);
	}




	/*
	void WindowSystem::swapBuffers()
	{

		SDL_GL_SwapWindow(m_window);
	}

	*/

	



}
