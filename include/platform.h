#pragma once

#include <cstdint>
#include "SDL3/SDL_video.h"
#include <memory>

namespace TheEngine
{

    enum class RenderingAPI
    {

        OPENGL_4_6,
        VULKAN_1_3
    };

    class IRenderingAPIContext
    {
    private:
		RenderingAPI m_renderingAPI;

    public:

        IRenderingAPIContext(const RenderingAPI renderingAPI) :m_renderingAPI(renderingAPI) {};
        RenderingAPI getRenderingAPI() const { return m_renderingAPI; };

		//Add specific context data later if needed

    };


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



    //This class will for MVP be tied to SDL


    enum class  EngineEventType : uint64_t
    {
        NONE,
        WINDOW_CLOSE,
        WINDOW_RESIZE,

        KEY_PRESSED,
        KEY_RELEASED,

        MOUSE_BUTTON_PRESSED,
        MOUSE_BUTTON_RELEASED,
        MOUSE_MOVE,
        MOUSE_SCROLLED

    };


    struct WindowResizeEvent
    {
      
        float dpiScale = 1.0f;

        //use this where ui is involved
        int32_t logicalPixelWidth = 0;
        int32_t logicalPixelHeight = 0;

        //use this in rendering system where swapchain size etc are involved
        int32_t physicalPixelWidth = 0;
        int32_t physicalPixelHeight = 0;

    };

    struct TouchEvent
    {
        float x = 0;
        float y = 0;

    };

    struct MouseMoveEvent
    {
        float x = 0;
        float y = 0;
        float xRel = 0;
        float yRel = 0;
	};

    struct EngineEvent
    {
        //is a uint64_t
        EngineEventType engineEventType = EngineEventType::NONE;
        union
        {
            WindowResizeEvent windowResizeEvent;
			MouseMoveEvent mouseMoveEvent;
            uint8_t padding[128];
        };
        EngineEvent() {};
    };






    class Platform
    {
    private:

        std::unique_ptr<IRenderingAPIContext> m_iRenderingAPIContext;

        SDL_Window* m_window = nullptr;

        bool m_keyStates[512];


        void enableOpenGLDebugging();

    public:

        Platform();

        //creates window and GPU API context
        void initialize(const RenderingAPI renderingAPI, const int width, const int height);

        bool isKeyPressed(int keycode) const;
        bool pollEvent(EngineEvent& outEvent);

        float getTimeInSeconds() const;


        void swapBuffers();

		void shutdown();


    };



}