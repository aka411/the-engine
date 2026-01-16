#pragma once
#include "core/ui_core_system.h"

#include "camera.h"
#include "core/ui_system.h"


namespace TheEngine::UI
{
	class UIRenderer
	{
	private:

		UICoreSystem& m_uiCoreSystem;//not owner

		UISystem& m_uiSystem;//not owner

		GLuint m_dummyVAO = 0;

		TheEngine::Camera m_uiCamera;
	
		void renderText();

	public:
	
		UIRenderer(UICoreSystem& uiCoreSystem, UISystem& uiSystem);

		void setViewportDimension(int width, int height);

		void renderUI();


	};


}