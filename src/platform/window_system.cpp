#include <SDL3/SDL_init.h>
#include <platform/window_system.h>
#include <rendering-system/api-backend/opengl/opengl_utils.h>
#include "glad/glad.h"
#include <cassert>
#include <iostream>



namespace TheEngine::Platform
{




	void WindowSystem::enableOpenGLDebugging()
	{

		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{

			glEnable(GL_DEBUG_OUTPUT);


			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);


			glDebugMessageCallback(DebugMessageCallback, NULL);


			glDebugMessageControl(
				GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
				0, NULL, GL_FALSE
			);

			std::cout << "OpenGL Debug Output successfully enabled." << std::endl;
		}
		else
		{

		}



	}

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
			m_iRenderingAPIContext = std::make_unique<OpenGLRenderingAPIContext>(gl_context);

			gladLoadGL();
			gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

			enableOpenGLDebugging();
			const GLubyte* rawString = glGetString(GL_RENDERER);

			if (rawString != nullptr)
			{
				m_gpuVendor = reinterpret_cast<const char*>(rawString);
			}

			break;
		}

		case RenderingAPI::VULKAN_1_3://also update to vulkan 1.4
		{
			////TODO : completed this later

			m_window = SDL_CreateWindow(
				"TheEngine(Vulkan 1.3)",
				engineConfiguration.windowWidth, engineConfiguration.windowHeight,
				SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
			);


			break;
		}

		default:
			break;
		}

	}








	void WindowSystem::swapBuffers()
	{

		SDL_GL_SwapWindow(m_window);
	}

	/*
	void Platform::shutdown()
	{
		//SDL_GL_DeleteContext(m_iRenderingAPIContext.get()-;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}
	*/



}
