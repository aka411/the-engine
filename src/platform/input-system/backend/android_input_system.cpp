#include <platform/input-system/backend/android_input_system.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace TheEngine::Platform
{

		AndroidInputSystem::AndroidInputSystem()
		{

		}


		void AndroidInputSystem::passInAndroidInputBuffer(android_input_buffer* inputBuffer)
		{


			/*
			switch ()
			{

			case:
			{
				break;
			}

			case:
			{

				break;
			}


			}


			*/

			android_app_clear_motion_events(inputBuffer);
			android_app_clear_key_events(inputBuffer);
		}
}