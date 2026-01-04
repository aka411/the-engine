#pragma once
#include <string>
#include "ui/core/ui_core_system.h"
#include "ui_text_generation_system.h"



//OBJECTIVE :  TO TRANSVERSE ALL TEXT AND UPDATE UITEXTRENDERMESH 
//RUNS EVERY FRAME

namespace TheEngine::UI
{

	class UITextSystem
	{

	private:

		UICoreSystem& m_uiCoreSystem;

		UITextGenerationSystem m_uiTextGenerationSystem;

		UIFontManager m_uiFontManager;//owner

		size_t uploadTextVertexData(std::byte* data, const size_t size, const UIBufferType uiBufferType);

		void updateText();

	public:


		UITextSystem(UICoreSystem& uiCoreSystem);
	

		void runTextSystem();

		//void loadFontData(const std::string& fontName, const std::string& fontMetricFilePath, const std::string& fontImageAtlasPath);
		//void loadFontData(const std::string& fontName, const std::byte* fontJsonData, uint64_t fontAtlasImageID);

	

		UIFontManager& getUIFontManager();


	};

}