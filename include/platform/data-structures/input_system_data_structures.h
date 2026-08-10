#pragma once
#include <cstdint>
#include <engine/engine_core_data_types.h>
namespace TheEngine
{




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

        WindowExtent extend;

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




}