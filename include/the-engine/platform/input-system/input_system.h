#pragma once
#include <vector>
#include <memory>

#include <platform/input-system/data-structures/input_system_data_structures.h>
#include <platform/input-system/i_input_system.h>

namespace TheEngine::Platform
{


	class InputSystem
	{

	private:

      
		std::unique_ptr<IInputSystem> m_inputSystem;
	public:

        
		InputSystem();

		bool isKeyPressed(int keycode) const;
		bool pollEvent(EngineEvent& outEvent);

		IInputSystem& getInputSystem();


	};





}