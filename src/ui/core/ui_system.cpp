#include "ui/core/ui_system.h"


namespace TheEngine::UI
{


	UI::UISystem::UISystem(UICoreSystem& uiCoreSystem) : 
		m_uiCoreSystem(uiCoreSystem),
		m_uiLayoutSystem(uiCoreSystem),
		m_uiMeshSystem(uiCoreSystem),
		m_uiTextSystem(uiCoreSystem)
	{


	}

	void UISystem::update()
	{

		m_uiMeshSystem.generateUIMeshesForDirty();
		m_uiLayoutSystem.updateLayout();

		m_uiTextSystem.runTextSystem();

	}


	UITextSystem& UISystem::getUITextSystem()
	{
		return m_uiTextSystem;
	}


	UIShaderManager& UISystem::getUIShaderManager()
	{
		return m_uiShaderManager;
	}

}