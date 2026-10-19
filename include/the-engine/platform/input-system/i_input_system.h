#pragma once
#include <vector>
#include <platform/input-system/data-structures/input_system_data_structures.h>

namespace TheEngine::Platform
{



	class IInputSystem
	{
	protected:

		std::vector<EngineEvent> m_inputQueue;
		bool m_keyStates[512]{false};//512 bytes on stack, 0.0005 % of stack limit, if stack is 1 MiB stack

	protected:
		IInputSystem() = default;
	public:



		bool isKeyPressed(int keycode);
		virtual bool pollEvent(EngineEvent& outEvent);


	};



}
