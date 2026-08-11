#pragma once

namespace TheEngine
{
	class Engine;
	struct EngineEvent;

	class Application
	{

	private:

		Engine* m_engine{ nullptr };

	public:


	
		virtual ~Application() = default;

		virtual void onInit(Engine& engine)
		{
			m_engine = &engine;
		};

		virtual void onEvent(const EngineEvent& event) = 0;
		virtual void onUpdate(const float dt) = 0;
		virtual void onShutDown() = 0;


	};
}