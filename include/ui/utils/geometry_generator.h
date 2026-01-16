#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>





class GeometryGenerator
{

private:


public:


	struct ShapeTrapezium
	{
		std::array<glm::vec2, 6> slopedRectangleLine;
		std::array<glm::vec2, 3> triangle;
		std::array<glm::vec2, 6> baseRectangle;
	};

	static std::array<glm::vec2, 6> generateLine(const glm::vec2& lineVector, const float lineWidth);


	//6 vertices for a rectangle since its made of two triangles
	static std::array<glm::vec2,6> generateRectangle(const float width, const float height);
	//yeah why have two methods one for line and one for rectangle why not use a common one ,
	// generateRectangle() method is for upright rectangle only and its faster than generateLine() method





	static ShapeTrapezium generateTrapezium(const float pointA, const float pointB, const float width);

	
	

};