#pragma once
#include <platform/input-system/i_input_system.h>

namespace TheEngine::Platform
{
	class DesktopInputSystem final : public IInputSystem
	{

	private:


	public:

		DesktopInputSystem() = default;

		virtual bool pollEvent(EngineEvent& outEvent) override;

	};

}