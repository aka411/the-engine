#include<core/engine.h>
#include<core/application.h>

int main(int argc, char** argv)
{

	TheEngine::Application* app = TheEngine::createApplication();

	assert(app != nullptr);

	TheEngine::EngineConfiguration engineConfiguration = app->getEngineConfiguration();
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