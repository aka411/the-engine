#pragma once
#include <engine/engine.h>
#include <engine/application.h>

namespace TheEngine
{
	class Application;
	struct EngineConfiguration;

	Application* createApplication(EngineConfiguration& engineConfiguration);

}