#pragma once

#include <cstdint>
#include "SDL3/SDL_video.h"
#include <memory>
#include <engine/engine_core_data_types.h>

namespace TheEngine::Platform
{




    class IRenderingAPIContext
    {
    private:

        RenderingAPI m_renderingAPI;
        SDL_Window* m_window = nullptr;

    public:

        IRenderingAPIContext(const RenderingAPI renderingAPI) :m_renderingAPI(renderingAPI) {};
        RenderingAPI getRenderingAPI() const { return m_renderingAPI; };

        //Add specific context data later if needed

    };




    
 //Move to its own class
    class OpenGLRenderingAPIContext : public IRenderingAPIContext
    {
    private:
        SDL_GLContext m_sdlGLContext;
    public:

        OpenGLRenderingAPIContext(SDL_GLContext sdlGLContext) :IRenderingAPIContext(RenderingAPI::OPENGL_4_6)
        {
            m_sdlGLContext = sdlGLContext;
        };

        SDL_GLContext getSDLGLContext()
        {
            return m_sdlGLContext;

        }

    };






	class WindowSystem
	{

	private:

        std::unique_ptr<IRenderingAPIContext> m_iRenderingAPIContext;
        std::string m_gpuVendor = "Unknown GPU Vendor";


        SDL_Window* m_window = nullptr;

        void enableOpenGLDebugging();


	public:


        //creates window and GPU API context
		WindowSystem(const EngineConfiguration& engineConfiguration);


		std::string getGPUVendor() const { return m_gpuVendor; };


		//swaps the front and back buffers, in opengl SDL does it for us , in vulkan its our job to do it
		void swapBuffers();



	};





}