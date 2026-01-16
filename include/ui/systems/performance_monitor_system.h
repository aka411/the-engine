#pragma once
#include "../core/ui_component.h"
#include "../core/ui_core_system.h"
#include "ecs.h"
#include "../builder/ui_element.h"

class PerformanceMonitorSystem
{

private:

	TheEngine::UI::UICoreSystem& m_uiCoreSystem;
	ECS::ECSEngine& m_ecsEngine;

	
	std::unique_ptr<TheEngine::UI::UIElement> m_fpsGraphElement;
	//change to ui element



public:
	PerformanceMonitorSystem(TheEngine::UI::UICoreSystem& uiCoreSystem);

	void setUp(TheEngine::UI::UIElement&& fpsGraphElement);

	void UpdateFPSMeter(const float deltaTime);
	void UpdateMemoryUsage(const size_t UsedBytes);





};
