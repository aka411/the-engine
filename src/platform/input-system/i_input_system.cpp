
#include <platform/input-system/i_input_system.h>

namespace TheEngine::Platform
{



	bool IInputSystem::isKeyPressed(int keycode)
	{
		return m_keyStates[keycode];
	}


	bool IInputSystem::pollEvent(EngineEvent& outEvent)
	{
		if (!m_inputQueue.empty())
		{
			outEvent = m_inputQueue.back();
			m_inputQueue.pop_back();
			return true;
		}
		return false;
	}



}

