#include "ui/text/ui_text_generation_system.h"


namespace TheEngine::UI
{


	std::vector<TextVertexData> UITextGenerationSystem::generateTextVertexData(const std::string& text, const RectBound& rectBound, const float fontSize, const glm::vec3& colour, const FontData& fontData)
	{

		/***cursor at top of ascender and not baseline ****/


	//..............................................Ascender
	//	|*(Cursor Origin)
	//	|
	//	|  We need to move the glyph by (Ascender in pixel - y1 in pixel)
	//	|  Only then does the characters line up correctly.
	//	| 
	//	|
	//	|
	//	|
	//----------------------------------BaseLine
	//  |
	//	|...........................................Desender
	//	|


		//TheMath::Vec2D offset = origin;
		const float rectWidth = rectBound.width;
		const float rectHeight = rectBound.height;

		/*
		std::vector<TheMath::Vec2D> baseVertices = {
	{-1.0f, 1.0f},  // Top-left
	{-1.0f, -1.0f},  // Bottom-left
	{1.0f, -1.0f},  // Bottom-right

	{1.0f, -1.0f},  // Bottom-right
	{1.0f, 1.0f},  // Top-right
	{-1.0f, 1.0f}   // Top-left
		};
		*/
		//TheMath::Vec2D rectCentreOffset{0.5f,0.5f};

		std::vector<glm::vec2> baseVertices = {
		{0.0f, 1.0f},  // Top-left
		{0.0f, 0.0f},  // Bottom-left
		{1.0f, 0.0f},  // Bottom-right

		{1.0f, 0.0f},  // Bottom-right
		{1.0f, 1.0f},  // Top-right
		{0.0f, 1.0f}   // Top-left
		};


		const FontData& m_fontData = fontData;


		float fontScale = m_fontData.fontScale * (fontSize / static_cast<float>(m_fontData.fontSizePx));

		float sdfPaddingPixels = m_fontData.sdfPaddingPx;


		float ascenderFontPixelUnits = m_fontData.ascenderFontUnits * fontScale;
		float descenderPixelUnits = m_fontData.descenderFontUnits * fontScale;
		float pixelDistScale = m_fontData.sdfSpreadScale;


		float lineGapPixelUnits = m_fontData.lineGapFontUnits * fontScale;

		const float lineHeight = ascenderFontPixelUnits - descenderPixelUnits + lineGapPixelUnits;
		float scaledSDFPadding = m_fontData.sdfPaddingPx * (fontSize / static_cast<float>(m_fontData.fontSizePx));



		glm::vec2 cursor(0.0f, lineHeight);

		std::vector<TextVertexData> textVertexDataList;
		textVertexDataList.reserve(text.size() * sizeof(TextVertexData)); // 6 vertices per character (2 triangles)
		
		
		for (const char& character : text)
		{



			const GlyphMetric& glyphMetric = m_fontData.glyphMetrics.at(character);



			float advanceHPixelUnit = glyphMetric.advance * fontScale;
			float leftSideBearingPixelUnit = glyphMetric.leftSideBearing * fontScale;
			float rightSideBearingPixelUnit = advanceHPixelUnit - (glyphMetric.x0 * fontScale);



			const float x0_scaled = glyphMetric.x0 * fontScale;
			const float y0_scaled = glyphMetric.y0 * fontScale;
			const float x1_scaled = glyphMetric.x1 * fontScale;
			const float y1_scaled = glyphMetric.y1 * fontScale;

			const float quadWidth = x1_scaled - x0_scaled + 2.0 * scaledSDFPadding;
			const float quadHeight = y1_scaled - y0_scaled + 2.0 * scaledSDFPadding;


			if (cursor.x + quadWidth > rectWidth)
			{
				cursor.y += lineHeight;
				cursor.x = 0;
			}
			if (cursor.y + quadHeight >= rectHeight)
			{
				return textVertexDataList;
			}





			float uvX = glyphMetric.uvX;
			float uvY = glyphMetric.uvY;  // UV coordinates (normalized 0.0-1.0)
			float uvW = glyphMetric.uvW;
			float uvH = glyphMetric.uvH;

			std::vector<glm::vec2>uvCoords = {
				{ uvX,uvY + uvH},
				{ uvX,uvY},
				{ uvX + uvW,uvY},

				{ uvX + uvW,uvY},
				{ uvX + uvW,uvY + uvH},
				{ uvX,uvY + uvH}
			};
			const float quadStartX = cursor.x + leftSideBearingPixelUnit;// -sdfPaddingPixels;


			const float quadStartY = cursor.y + y0_scaled;//(ascenderFontPixelUnits - y1_scaled);

			glm::vec2 quadOffset(quadStartX, quadStartY);

			for (size_t i = 0; i < baseVertices.size(); ++i)
			{
				TextVertexData textVertexData;
				const glm::vec2 vertex = baseVertices[i];

				// Scale the vertex
				float scaledX = vertex.x * quadWidth;
				float scaledY = vertex.y * quadHeight;




				// Translate the scaled vertex

				float finalX = scaledX + quadOffset.x;
				float finalY = scaledY + quadOffset.y - quadHeight;

				textVertexData.position = glm::vec3{ finalX,finalY,0.0f };// I might use z axis in future
				textVertexData.uv = glm::vec2{ uvCoords[i].x,uvCoords[i].y };
				textVertexData.color = glm::vec4{ colour.r,colour.g,colour.b,1.0f };
				textVertexDataList.push_back(textVertexData);
			}





			cursor.x += advanceHPixelUnit;



		}

		//std::cout << std::endl;
		//for (auto& textVertexData : textVertexDataList)
		//{

		//	std::cout << std::to_string(textVertexData.position.x) << " " << std::to_string(textVertexData.position.y) << std::endl;
		//}


		return textVertexDataList;
	}








	

}