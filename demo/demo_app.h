#pragma once

#include "i_application.h"
#include <ui/include/systems/performance_monitor_system.h>
#include <camera.h>
#include <engine/engine_core.h>


class DemoApp : public TheEngine::IApplication
{	
private:
	TheEngine::Camera m_camera;
	
	std::unique_ptr<TheEngine::EngineCore> m_engineCore;
	TheEngine::Platform m_platform;
	std::unique_ptr <PerformanceMonitorSystem> m_performanceMonitorSystem;


	void processInput(float deltaTime);
public:

	 DemoApp();
	~DemoApp() override;

	void onStart() override;

	void onUpdate(float dt) override;

	void onRender() override;

	void onEvent(TheEngine::EngineEvent& event) override;


	void run() override;

};