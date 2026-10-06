

#include <jni.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>

#include<core/engine.h>
#include<core/application.h>

#include <platform/input-system/backend/android_input_system.h>
#include <platform/window-system/backend/android_vulkan_window.h>





	extern "C"
	{



		struct EngineContext
		{
			TheEngine::EngineConfiguration engineConfiguration{};
			//std::unique_ptr<TheEngine::Application> app;
            TheEngine::Application* app{nullptr};
			std::unique_ptr <TheEngine::Engine> engine;

		};

		inline EngineContext* getEngineContext(android_app* pApp)
		{
			if (!pApp->userData)
			{
				assert(false);
			}
			return reinterpret_cast<EngineContext*>(pApp->userData);
		}

		inline void checkIfEngineContextIsValid(EngineContext* engineContext)
		{
			if (!engineContext)
			{
				assert(false);
			}

		}


		void onInputEvent(struct android_app* app)
		{
		

			auto inputBuffer = android_app_swap_input_buffers(app);

			if (inputBuffer) 
			{

				EngineContext* engineContext = getEngineContext(app);

				auto& androidInputSystem = reinterpret_cast<TheEngine::Platform::AndroidInputSystem&>(engineContext->engine->getPlatform().getInputSystem().getInputSystem());
				androidInputSystem.passInAndroidInputBuffer(inputBuffer);
				
			}


		}




		void handle_cmd(android_app* pApp, int32_t cmd) 
		{
			EngineContext* engineContext = getEngineContext(pApp);

			switch (cmd) 
			{
			case APP_CMD_INIT_WINDOW:
			{
				if (engineContext->engine == nullptr)
				{

					TheEngine::Application* app = TheEngine::createApplication();

					assert(app != nullptr);
			
					engineContext->engineConfiguration = app->getEngineConfiguration();

					TheEngine::EngineConfiguration& engineConfiguration = engineContext->engineConfiguration;

					engineConfiguration.androidApp = pApp;

					engineContext->app = app;
					engineContext->engine = std::make_unique<TheEngine::Engine>(engineConfiguration);

					app->onInit(*engineContext->engine);


				}
				else
				{

					auto& androidVulkanWindow = static_cast<TheEngine::Platform::AndroidVulkanWindow&>(engineContext->engine->getPlatform().getWindowSystem().getWindow());
					androidVulkanWindow.onWindowCreated();

				}

			}
				break;

			case APP_CMD_TERM_WINDOW:
			{
				auto& androidVulkanWindow = static_cast<TheEngine::Platform::AndroidVulkanWindow&>(engineContext->engine->getPlatform().getWindowSystem().getWindow());
				androidVulkanWindow.onWindowDestroyed();
				break;
			}

			case APP_CMD_DESTROY:

				if (pApp->userData) 
				{
					//
					auto* pEngineContext = getEngineContext(pApp);


					//TODO : Implement corect destruction later
					delete pEngineContext->app;
					pEngineContext->engine.reset();
					//delete pEngineContext;

					pApp->userData = nullptr;
				}
				break;
			default:
				break;
			}
		}


		bool motion_event_filter_func(const GameActivityMotionEvent* motionEvent) 
		{
			auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
			return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
				sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
		}


		void android_main(struct android_app* pApp) 
		{


			EngineContext engineContext;

			pApp->onAppCmd = handle_cmd;

			pApp->userData = &engineContext;


			android_app_set_motion_event_filter(pApp, motion_event_filter_func);


			do {
	
				bool done = false;
				while (!done) 
				{
					// 0 is non-blocking.
					int timeout = 0;
					int events;

					android_poll_source* pSource;

					int result = ALooper_pollOnce(timeout, 
						nullptr,
						&events,
						reinterpret_cast<void**>(&pSource));

					switch (result) 
					{

					case ALOOPER_POLL_TIMEOUT:

						[[clang::fallthrough]];

					case ALOOPER_POLL_WAKE:
	
						done = true;
						break;

					case ALOOPER_EVENT_ERROR:

						//aout << "ALooper_pollOnce returned an error" << std::endl;
						break;

					case ALOOPER_POLL_CALLBACK:
						break;

					default:
						if (pSource) 
						{
							pSource->process(pApp, pSource);
						}
					}
				}


				if (pApp->userData)
				{

					onInputEvent(pApp);

					auto* pEngineContext = getEngineContext(pApp);
					checkIfEngineContextIsValid(pEngineContext);
                    if(!pEngineContext->engine || !pEngineContext->app)
                    {
                        continue;
                    }
					pEngineContext->engine->run(*pEngineContext->app);

					//TODO : 
					pEngineContext->app->onUpdate(0.001f);
				
				}
			} while (!pApp->destroyRequested);
		}
	}



