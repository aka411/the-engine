#pragma once
#include <core/engine.h>
#include <core/application.h>




	class CompileTestStub : public TheEngine::Application
	{


	public:


		CompileTestStub();

		virtual TheEngine::EngineConfiguration getEngineConfiguration() override 
		{
			return TheEngine::EngineConfiguration{};
		};

		virtual void onInit(TheEngine::Engine& engine) override {};

		virtual void onEvent(const TheEngine::EngineEvent& event) override {};
		virtual void onUpdate(const float dt) override {};
		virtual void onShutDown() override {};



	};
