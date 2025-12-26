#pragma once
#include "platform.h"


namespace TheEngine
{

enum class EngineState
{
    RUNNING,
    PAUSED,
};



    class IApplication
    {
    protected:

        //Engine m_engine;
        bool m_isRunning = true;


    public:

        IApplication() = default;
        virtual ~IApplication() = default;

        // The "Hook" methods for the dev
        virtual void onStart() = 0;
        virtual void onUpdate(float dt) = 0;
        virtual void onRender() = 0;
		virtual void onEvent(EngineEvent& event) = 0;

        // The main engine loop
        virtual void run() = 0;


    };


}