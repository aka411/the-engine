#pragma once
#include <platform/input-system/i_input_system.h>

struct android_input_buffer;

namespace TheEngine::Platform
{
	class AndroidInputSystem final : public IInputSystem
	{
	private:

	public:

		AndroidInputSystem();




		//Android specific Method
		void passInAndroidInputBuffer(android_input_buffer* inputBuffer);


	};



}