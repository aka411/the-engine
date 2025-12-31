#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "ui/text/json_font_parser.h"



//will compiler add padding , well its all just floats so should be fine
struct TextVertexData
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 uv;

};


struct RectBound
{
	//glm::vec2 topLeft;
	//glm::vec2 bottomRight;

	float width;
	float height;


};

namespace TheEngine::UI
{

	class UITextGenerationSystem
	{

	private:



	public:

		std::vector<TextVertexData> generateTextVertexData(const std::string& text, const RectBound& rectBound, const float fontSize,const glm::vec3& colour, const FontData& fontData);
		
	};





}