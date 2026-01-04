#include "platform.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>

#include "glad/glad.h"
#include "rendering-engine/opengl-backend/opengl_utils.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

namespace TheEngine
{
	void Platform::enableOpenGLDebugging()
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

	Platform::Platform() :
		m_keyStates{ false }
	{

		//std::fill(std::begin(m_keyStates), std::end(m_keyStates), false);


	}


	void Platform::initialize(const  RenderingAPI renderingAPI, const int width, const int height)
	{


		SDL_Init(SDL_INIT_VIDEO);

	

		switch (renderingAPI)
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
				"Opengl Rendering Engine(openGL 4.6)",
				width, height,
				SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
			);


			//Bad
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

		case RenderingAPI::VULKAN_1_3:
		{
			////TODO : completed this later

			m_window = SDL_CreateWindow(
				"Vukan Rendering Engine(Vulkan 1.3)",
				width, height,
				SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
			);


			break;
		}

		default:
			break;
		}


	}



	bool Platform::isKeyPressed(int keycode) const
	{
		if (keycode < 0 || keycode >= 512)
			return false;
		return m_keyStates[keycode];
	}



	bool Platform::pollEvent(EngineEvent& outEvent)
	{
		SDL_Event sdlEvent;

		if (SDL_PollEvent(&sdlEvent))
		{
			// Translation logic happens here
			switch (sdlEvent.type)
			{
			case SDL_EVENT_WINDOW_RESIZED:
			{
				outEvent.engineEventType = EngineEventType::WINDOW_RESIZE;

				// Physical Pixels, not logical pixels, it has DPI scaling applied
				outEvent.windowResizeEvent.physicalPixelWidth = sdlEvent.window.data1;
				outEvent.windowResizeEvent.physicalPixelHeight = sdlEvent.window.data2;
				
				/// Logical pixels, 
				int logicalW, logicalH;
				SDL_GetWindowSize(m_window, &logicalW, &logicalH);
				outEvent.windowResizeEvent.logicalPixelWidth = logicalW;
				outEvent.windowResizeEvent.logicalPixelHeight = logicalH;
				return true;
			}
			break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				outEvent.engineEventType = EngineEventType::WINDOW_CLOSE;

				return true;
			}
			break;



			case SDL_EVENT_KEY_DOWN:
			{
				if (sdlEvent.key.scancode < 512 && sdlEvent.key.scancode >= 0)
				{
					outEvent.engineEventType = EngineEventType::KEY_PRESSED;
					m_keyStates[sdlEvent.key.scancode] = true;
					return true;
				}
			}
			break;



			case SDL_EVENT_KEY_UP:
			{
				if (sdlEvent.key.scancode < 512 && sdlEvent.key.scancode >= 0)
				{
					outEvent.engineEventType = EngineEventType::KEY_RELEASED;
					m_keyStates[sdlEvent.key.scancode] = false;
					return true;
				}
			}
			break;



			case SDL_EVENT_MOUSE_MOTION:
			{
				outEvent.engineEventType = EngineEventType::MOUSE_MOVE;
				outEvent.mouseMoveEvent.x = static_cast<float>(sdlEvent.motion.x);
				outEvent.mouseMoveEvent.y = static_cast<float>(sdlEvent.motion.y);
				outEvent.mouseMoveEvent.xRel = static_cast<float>(sdlEvent.motion.xrel);
				outEvent.mouseMoveEvent.yRel = static_cast<float>(sdlEvent.motion.yrel);
				return true;




			}
			break;
			}


			
		}

		return false;
	}


	float Platform::getTimeInSeconds() const
	{

		return SDL_GetTicks() / 1000.0f;

	}


	FileData Platform::readFile(const std::string& pathToFile)
	{


		FileData fileData;


		std::ifstream file(pathToFile, std::ios::binary | std::ios::ate);
		if (!file) return fileData;

		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (size > 0)
		{

			void* rawBuffer = std::malloc(static_cast<size_t>(size));

			if (rawBuffer)
			{
				
				if (file.read(reinterpret_cast<char*>(rawBuffer), size))
				{
					fileData.size = static_cast<size_t>(size);
					fileData.data = {
						//reinterpret cast?
						static_cast<std::byte*>(rawBuffer),
						[](void* p) { std::free(p); }
					};
				}
				else
				{
					std::free(rawBuffer);
					assert(false && "Failed to read file data.");
				}
			}
		}
		else
		{
			assert(false && "File is empty or could not determine size.");
		}

		return fileData;

	}


	void Platform::swapBuffers()
	{

		SDL_GL_SwapWindow(m_window);
	}

	void Platform::shutdown()
	{
		//SDL_GL_DeleteContext(m_iRenderingAPIContext.get()-;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}


}
