#pragma once
#include "input_system.h"
#include "file_system.h"
#include "window_system.h"


namespace TheEngine
{
    struct EngineConfiguration;
}

namespace TheEngine::Platform
{



  
    class Platform
    {


    private:

        WindowSystem m_windowSystem;//Mostly for engines internal use

        InputSystem m_inputSystem;//Exposed to outside world
        FileSystem m_fileSystem;//Exposed to outside world

    public:


        Platform(const TheEngine::EngineConfiguration& engineConfiguration);

        WindowSystem& getWindowSystem();

        InputSystem& getInputSystem();
        FileSystem& getFileSystem();


    };



}