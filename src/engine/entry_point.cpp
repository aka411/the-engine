#include <engine/entry_point.h>





	int main(int argc, char** argv)
	{
		TheEngine::EngineConfiguration engineConfiguration;
		TheEngine::Application* app = TheEngine::createApplication(engineConfiguration);

		assert(app != nullptr);

		TheEngine::Engine engine(engineConfiguration);

		app->onInit(engine);

		while (engine.isEngineRunning())
		{
			engine.run(*app);
			app->onUpdate(0.001f);
		}


		delete app;
		return 0;
	}