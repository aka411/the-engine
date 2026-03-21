#include <platform/platform.h>


namespace TheEngine::Platform
{



    Platform::Platform(const TheEngine::EngineConfiguration& engineConfiguration) :
        m_fileSystem(engineConfiguration),
        m_inputSystem(),

        m_windowSystem(engineConfiguration)
    {

        

    }


    WindowSystem& Platform::getWindowSystem()
    {
        return m_windowSystem;
    }



    InputSystem& Platform::getInputSystem()
    {
        return m_inputSystem;
    }




    FileSystem& Platform::getFileSystem()
    {

        return m_fileSystem;
    }




}
