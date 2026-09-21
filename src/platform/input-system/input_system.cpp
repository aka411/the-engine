#include <platform/input-system/input_system.h>

#ifdef PLATFORM_PC

#include <platform/input-system/backend/desktop_input_system.h>

#elif PLATFORM_ANDROID

#include <platform/input-system/backend/android_input_system.h>

#endif

namespace TheEngine::Platform
{


	InputSystem::InputSystem()
	{


#ifdef PLATFORM_PC

		m_inputSystem = std::make_unique<DesktopInputSystem>();

#elif PLATFORM_ANDROID

		m_inputSystem = std::make_unique<AndroidInputSystem>();

#elif PLATFORM_TEST
		//Do nothing
#else
	#error("No Platform Defined")
#endif
	}


	bool InputSystem::isKeyPressed(int keycode) const
	{
		return m_inputSystem->isKeyPressed(keycode);
	}

	bool InputSystem::pollEvent(EngineEvent& outEvent)
	{
		return m_inputSystem->pollEvent(outEvent);
	}

	IInputSystem& InputSystem::getInputSystem()
	{
		return *m_inputSystem;
	}

}