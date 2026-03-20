#pragma once
#include <vector>

namespace TheEngine::Platform
{

    //This class will for MVP be tied to SDL

    //how about a type and sub type like Event -> KeyBoard, Mosue, Touch.    SubEvent -> Move , Release, Press, 
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

        // USE ONLY POD TYPES IN THE UNION TO AVOID CONSTRUCTOR/DESTRUCTOR ISSUES
        union
        {
            WindowResizeEvent windowResizeEvent;
            MouseMoveEvent mouseMoveEvent;
            uint8_t padding[128] = { 0 };
        };
        EngineEvent() {};
    };

	class InputSystem
	{

	private:

        bool m_keyStates[512];
        std::vector<EngineEvent> m_inputQueue;

	public:

        //who decided who can consume the events
        
		InputSystem();

		bool isKeyPressed(int keycode) const;
		bool pollEvent(EngineEvent& outEvent);

        void resetKeyState();


	};





}