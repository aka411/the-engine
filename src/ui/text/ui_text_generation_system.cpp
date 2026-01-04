#include "ui/text/ui_text_generation_system.h"
#include <iostream>


namespace TheEngine::UI
{


	std::vector<TextVertexData> UITextGenerationSystem::generateTextVertexData(const std::string& text, const RectBound& rectBound, const float fontSize, const glm::vec3& colour, const FontData& fontData) const
	{
		/******
		NOTE
		It can get confusing here , Text placment takes in Y axis down coordinate system
		but the Font metrics are defined in Y axis up coordinate system.


		
		
		*****/



		/***cursor at top of ascender and not baseline ****/

	//  Y+
	//..^............................................Ascender
	//	|*<----(Cursor Origin)
	//	|
	//	|  We need to move the glyph by (Ascender in pixel - y0 in pixel)(Along Y-Axis)
	//	|  Only then does the characters line up correctly.
	//	| 
	//	|
	//	|
	//	|
	//----------------------------------BaseLine
	//  |
	//	|...........................................Desender
	//	|


		/*
		
		Glyph Metric Coordinates System
		    Y+
			^
			|
			|     
	   y1...|....._________
			|     |       |
			|     |       |  
			|     |       |
			|     |       |
	   y0...|.....|_______|
			|     :       :
			|     :       :
			+-----:-------:-------------> X+
				  x0      x1
		
		
		
		
		*/


		const float rectWidth = rectBound.width;
		const float rectHeight = rectBound.height;







		//UV coordinates range from 0.0 to 1.0
		//two of the vertex of quad will be at (0,0) and other at (1,1)
	    const std::vector<glm::vec2> baseVertices = {
		{0.0f, 1.0f},  // Top-left
		{0.0f, 0.0f},  // Bottom-left
		{1.0f, 0.0f},  // Bottom-right

		{1.0f, 0.0f},  // Bottom-right
		{1.0f, 1.0f},  // Top-right
		{0.0f, 1.0f}   // Top-left
		};
		

		const FontData& m_fontData = fontData;


		const float fontScale = m_fontData.fontScale * (fontSize / static_cast<float>(m_fontData.fontSizePx));

		const float sdfPaddingPixels = m_fontData.sdfPaddingPx;


		const float ascenderFontPixelUnits = m_fontData.ascenderFontUnits * fontScale;
		const float descenderPixelUnits = m_fontData.descenderFontUnits * fontScale;
		const float pixelDistScale = m_fontData.sdfSpreadScale;


		float lineGapPixelUnits = m_fontData.lineGapFontUnits * fontScale;

		//const float lineHeight = ascenderFontPixelUnits -descenderPixelUnits + lineGapPixelUnits;
		const float lineHeight = ascenderFontPixelUnits -descenderPixelUnits + lineGapPixelUnits;
		const float baselineToBaselineDistance = lineHeight;
		const float ascenderToAscenderDistance = lineHeight;

		const float scaledSDFPadding = m_fontData.sdfPaddingPx * (fontSize / static_cast<float>(m_fontData.fontSizePx));


		
		glm::vec2 cursor(0.0f, 0.0f);

		std::vector<TextVertexData> textVertexDataList;
		textVertexDataList.reserve(text.size() * sizeof(TextVertexData)); // 6 vertices per character (2 triangles)
		
		
		for (const char& character : text)
		{


			if(character == '\n')
			{
				cursor.y += ascenderToAscenderDistance;
				cursor.x = 0;
				continue;
			}

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
				cursor.y += ascenderToAscenderDistance;
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

			const std::vector<glm::vec2>uvCoords = {
				{ uvX,uvY + uvH},
				{ uvX,uvY},
				{ uvX + uvW,uvY},

				{ uvX + uvW,uvY},
				{ uvX + uvW,uvY + uvH},
				{ uvX,uvY + uvH}
			};


			const float quadStartX = cursor.x +leftSideBearingPixelUnit+ x0_scaled;

			//                         |-----------gets To BaseLine--------------------------------||--per glyph descend-|
			//const float quadStartY = cursor.y + (ascenderFontPixelUnits - (y1_scaled - y0_scaled)) - y0_scaled;
			

			float baselineY = cursor.y + ascenderFontPixelUnits;
			const float quadStartY = baselineY - y1_scaled;



			glm::vec2 quadOffset(quadStartX, quadStartY);

			for (size_t i = 0; i < baseVertices.size(); ++i)
			{
				TextVertexData textVertexData;
				const glm::vec2& vertex = baseVertices[i];

				// Scale the vertex
				const float scaledX = vertex.x * quadWidth;
				const float scaledY = vertex.y * quadHeight;




				// Translate the scaled vertex

				const float finalX = scaledX + quadOffset.x;
				const float finalY = scaledY + quadOffset.y;

				textVertexData.position = glm::vec3{ finalX,finalY,0.0f };// I might use z axis in future
				textVertexData.uv = glm::vec2{ uvCoords[i].x,uvCoords[i].y };
				textVertexData.color = glm::vec4{ colour.r,colour.g,colour.b,1.0f };
				textVertexDataList.push_back(textVertexData);
			}





			cursor.x += advanceHPixelUnit;



		}

		return textVertexDataList;
	}








	

}