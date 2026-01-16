#pragma once
#include "ui_core_system.h"
#include "../systems/ui_layout_system.h"
#include "ui/systems/ui_mesh_system.h"
#include "ui/text/ui_text_system.h"

namespace TheEngine::UI
{
	class UISystem
	{

	private:

		UICoreSystem& m_uiCoreSystem;//not owner

		//UI Internal Systems
		UILayoutSystem m_uiLayoutSystem;//owner
		UIMeshSystem m_uiMeshSystem;//owner

		UIShaderManager m_uiShaderManager;//owner 

		UITextSystem m_uiTextSystem;//owner

	public:

		//const UIBuilder uiBuilder;//but it in engine directly



		UISystem(UICoreSystem& uiCoreSystem);


		void update();//run per frame systems update



		UITextSystem& getUITextSystem();

		UIShaderManager& getUIShaderManager();


	};

}