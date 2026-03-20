#include <platform/input_system.h>
#include <SDL3/SDL_events.h>


namespace TheEngine::Platform
{


	


	InputSystem::InputSystem():
		m_keyStates{ false }
	{

	}

	bool InputSystem::isKeyPressed(int keycode) const
	{
		if (keycode < 0 || keycode >= 512)
			return false;
		return m_keyStates[keycode];
	}



	bool InputSystem::pollEvent(EngineEvent& outEvent)
	{
		SDL_Event sdlEvent;

		if (SDL_PollEvent(&sdlEvent))
		{
			// Translation logic happens here
			switch (sdlEvent.type)
			{
			case SDL_EVENT_WINDOW_RESIZED:
			{
				outEvent.engineEventType = EngineEventType::WINDOW_RESIZE;

				// Physical Pixels, not logical pixels, it has DPI scaling applied
				outEvent.windowResizeEvent.physicalPixelWidth = sdlEvent.window.data1;
				outEvent.windowResizeEvent.physicalPixelHeight = sdlEvent.window.data2;

				/// Logical pixels, 
				int logicalW, logicalH;
				//SDL_GetWindowSize(m_window, &logicalW, &logicalH);
				outEvent.windowResizeEvent.logicalPixelWidth = logicalW;
				outEvent.windowResizeEvent.logicalPixelHeight = logicalH;
				return true;
			}
			break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				outEvent.engineEventType = EngineEventType::WINDOW_CLOSE;

				return true;
			}
			break;



			case SDL_EVENT_KEY_DOWN:
			{
				if (sdlEvent.key.scancode < 512 && sdlEvent.key.scancode >= 0)
				{
					outEvent.engineEventType = EngineEventType::KEY_PRESSED;
					m_keyStates[sdlEvent.key.scancode] = true;
					return true;
				}
			}
			break;



			case SDL_EVENT_KEY_UP:
			{
				if (sdlEvent.key.scancode < 512 && sdlEvent.key.scancode >= 0)
				{
					outEvent.engineEventType = EngineEventType::KEY_RELEASED;
					m_keyStates[sdlEvent.key.scancode] = false;
					return true;
				}
			}
			break;



			case SDL_EVENT_MOUSE_MOTION:
			{
				outEvent.engineEventType = EngineEventType::MOUSE_MOVE;
				outEvent.mouseMoveEvent.x = static_cast<float>(sdlEvent.motion.x);
				outEvent.mouseMoveEvent.y = static_cast<float>(sdlEvent.motion.y);
				outEvent.mouseMoveEvent.xRel = static_cast<float>(sdlEvent.motion.xrel);
				outEvent.mouseMoveEvent.yRel = static_cast<float>(sdlEvent.motion.yrel);
				return true;




			}
			break;
			}



		}

		return false;
	}



}