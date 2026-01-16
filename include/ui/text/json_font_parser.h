#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>




struct GlyphMetric
{
	//char character;

	// 1. FOR LAYOUT (Needs to be scaled to screen pixels at render time)
	float advance;      // Horizontal advance (in Font Units)
	float leftSideBearing; // Horizontal offset from cursor (in Font Units)

	float x0, y0, x1, y1; // Bounding box of the *outline* (in Font Units) 
	//Not accounting for SDF padding 
	
	
	// 2. FOR QUAD TEXTURING (The data from the atlas)
	float uvX, uvY, uvW, uvH; // UV coordinates (normalized )

	// 3. SDF-SPECIFIC METRICS
	//int xPacked, yPacked; // Packed position in atlas (in pixels)
	//int sdfWPx, sdfHPx; // Actual pixel size of the SDF in the atlas 
};





struct FontData
{
	// Global Metrics
	int atlasWidthPx = 0;
	int atlasHeightPx = 0; // The height of the actual content

	int fontSizePx = 0;
	float fontScale = 0;
	int sdfPaddingPx = 0;
	int sdfSpreadScale = 0;
	int ascenderFontUnits = 0;
	int descenderFontUnits = 0;
	int lineGapFontUnits = 0;

	std::unordered_map<char, GlyphMetric> glyphMetrics;
	
};


class JsonFontParser
{
private:


	FontData parseFontDataFromJson(const nlohmann::json& json);

public:

	JsonFontParser();

	FontData getFontData(const std::string& filePath);

	//
	FontData getFontData(const std::byte* jsonData, size_t dataSize);

};

