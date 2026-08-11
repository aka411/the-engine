#include <platform/input_system.h>
#include <SDL3/SDL_events.h>
#include <assert.h>


namespace TheEngine::Platform
{





	InputSystem::InputSystem() :
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

				//Logical pixels, it has DPI scaling applied
				////SDL_GetWindowSize(m_window, &logicalW, &logicalH);
				outEvent.windowResizeEvent.extend.logical.width = sdlEvent.window.data1;//This is logical
				outEvent.windowResizeEvent.extend.logical.height = sdlEvent.window.data2;

				/// int width, height;
				//SDL_GetWindowSizeInPixels(window, &width, &height);
				int physicalW, physicalH;
				//outEvent.windowResizeEvent.extend.framebuffer.width
				outEvent.windowResizeEvent.extend.framebuffer.width = sdlEvent.window.data1;//This is logical
				outEvent.windowResizeEvent.extend.framebuffer.height = sdlEvent.window.data2;
				return true;
			}
			break;

			//case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			//{
			//	assert(false && "SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED not handled");
			//	return true;
			//}
			//break;

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


	void InputSystem::resetKeyState()
	{
		memset(&m_keyStates[0], false, 512);

	}
}