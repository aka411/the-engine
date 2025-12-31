#pragma once
#include "json_font_parser.h"
#include <string>
#include "platform.h"

namespace TheEngine::UI
{

	struct FontAsset
	{
		FontData fontData;
		uint64_t fontAtlasImageID;
	};

	class UIFontManager
	{

	private:

		std::unordered_map<std::string, FontAsset> m_loadedFonts;

	public:

		UIFontManager();

		//void loadFont(const std::string& fontName, const std::string& fontJsonFilePath, uint64_t fontAtlasImageID);
		void loadFont(const std::string& fontName,const FileData& fontJsonData, uint64_t fontAtlasImageID);

		const FontAsset* getFontAsset(const std::string& fontName) const;

		
	};




}