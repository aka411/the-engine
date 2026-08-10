#pragma once
#include <vector>

#include <platform/data-structures/input_system_data_structures.h>

namespace TheEngine::Platform
{


	class InputSystem
	{

	private:

        bool m_keyStates[512];//512 bytes on stack, 0.0005 % of stack limit, if stack is 1 MiB stack
        std::vector<EngineEvent> m_inputQueue;

	public:

        
		InputSystem();

		bool isKeyPressed(int keycode) const;
		bool pollEvent(EngineEvent& outEvent);

        void resetKeyState();


	};





}