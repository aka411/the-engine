#include "ui/text/json_font_parser.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"



FontData JsonFontParser::parseFontDataFromJson(const nlohmann::json& json)
{

	FontData fontData;


	fontData.atlasWidthPx = json.at("atlasWidthPx").get<int>();
	fontData.atlasHeightPx = json.at("atlasHeightPx").get<int>();

	fontData.fontSizePx = json.at("fontSizePx").get<int>();
	fontData.fontScale = json.at("fontScale").get<float>();;
	fontData.sdfPaddingPx = json.at("sdfPaddingPx").get<int>();
	fontData.sdfSpreadScale = json.at("sdfSpreadScale").get<int>();
	fontData.ascenderFontUnits = json.at("ascenderFontUnits").get<int>();
	fontData.descenderFontUnits = json.at("descenderFontUnits").get<int>();
	fontData.lineGapFontUnits = json.at("lineGapFontUnits").get<int>();



	const nlohmann::json& glyphMetrics = json.at("glyphMetrics");

	for (const auto& glyphMetricJson : glyphMetrics)
	{

		std::string character = glyphMetricJson.at("character").get<std::string>();

		if (character.empty())
		{
			continue;
		}

		GlyphMetric glyphMetric;




		//  Layout Metrics (Floats)
		glyphMetric.advance = glyphMetricJson.at("advance").get<float>();
		glyphMetric.leftSideBearing = glyphMetricJson.at("leftSideBearing").get<float>();
		glyphMetric.x0 = glyphMetricJson.at("x0").get<float>();
		glyphMetric.y0 = glyphMetricJson.at("y0").get<float>();
		glyphMetric.x1 = glyphMetricJson.at("x1").get<float>();
		glyphMetric.y1 = glyphMetricJson.at("y1").get<float>();

		//  Texturing Metrics (Floats)
		glyphMetric.uvX = glyphMetricJson.at("uvX").get<float>();
		glyphMetric.uvY = glyphMetricJson.at("uvY").get<float>();
		glyphMetric.uvW = glyphMetricJson.at("uvW").get<float>();
		glyphMetric.uvH = glyphMetricJson.at("uvH").get<float>();

		fontData.glyphMetrics[character[0]] = glyphMetric;


	}



	return fontData;
}

JsonFontParser::JsonFontParser()
{




}

FontData JsonFontParser::getFontData(const std::string& filePath)
{



	//read file handle error 
	std::ifstream inputJsonFile(filePath, std::ios::in);


	if (!inputJsonFile.is_open())
	{
		assert(false && "Failed to open font JSON file.");
		return FontData();
	}



	try
	{
		nlohmann::json json;
		inputJsonFile >> json;


		return parseFontDataFromJson(json);


	}
	catch (nlohmann::json::parse_error& e)
	{
		std::cerr << "JSON Parse Error: " << e.what() << std::endl;
		assert(false && "Failed to parse font JSON file.");
		return FontData();
	}


	inputJsonFile.close();

}



FontData JsonFontParser::getFontData(const std::byte* jsonData, size_t dataSize)
{
	//error handling?
	nlohmann::json json;

	

	json = nlohmann::json::parse(reinterpret_cast<const std::byte*>(jsonData), reinterpret_cast<const std::byte*>(jsonData) + dataSize);

	return	parseFontDataFromJson(json);
}