#include "ui/text/ui_font_manager.h"



namespace TheEngine::UI
{

	/*
	void UIFontManager::loadFont(const std::string& fontName, const std::string& fontJsonFilePath, uint64_t fontAtlasImageID)
	{
		JsonFontParser jsonFontParser;
		FontData fontData = jsonFontParser.getFontData(fontJsonFilePath);
		FontAsset fontAsset;
		fontAsset.fontData = fontData;
		fontAsset.fontAtlasImageID = fontAtlasImageID;
		m_loadedFonts[fontName] = fontAsset;
	}*/
	UIFontManager::UIFontManager()
	{


	}

	void UIFontManager::loadFont(const std::string& fontName, const FileData& fontJsonData, uint64_t fontAtlasImageID)
	{


		JsonFontParser jsonFontParser;
		FontData fontData = jsonFontParser.getFontData(fontJsonData.data.get(), fontJsonData.size);
		FontAsset fontAsset;
		fontAsset.fontData = fontData;
		fontAsset.fontAtlasImageID = fontAtlasImageID;
		m_loadedFonts[fontName] = fontAsset;

	}



	const FontAsset* UIFontManager::getFontAsset(const std::string& fontName) const
	{
		const auto& it = m_loadedFonts.find(fontName);
		if (it != m_loadedFonts.end())
		{
			return &it->second;
		}
		return nullptr;
	}





}