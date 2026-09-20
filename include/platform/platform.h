#pragma once
#include <platform/window-system/window_system.h>
#include <platform/file-system/file_system.h>
#include <platform/input-system/input_system.h>


namespace TheEngine
{
    struct EngineConfiguration;
}


namespace TheEngine::Platform
{



  
    class Platform
    {


    private:

        WindowSystem m_windowSystem;

        InputSystem m_inputSystem;
        FileSystem m_fileSystem;

    public:


        Platform(const TheEngine::EngineConfiguration& engineConfiguration);

        WindowSystem& getWindowSystem();

        InputSystem& getInputSystem();
        FileSystem& getFileSystem();


    };



}