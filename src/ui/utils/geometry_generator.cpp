#include "ui/utils/geometry_generator.h"

    



std::array<glm::vec2, 6> GeometryGenerator::generateLine(const glm::vec2& lineVector, const float lineWidth)
{

	/*        ^Normal
	          |
	          |
	          |     lineVector
	----------|------------>
	
	*/

	const glm::vec2 normal = glm::normalize(glm::vec2(-lineVector.y, lineVector.x));//unit normal

	/*
	A---------------------B      BC = AD = width
	|                     |       
   -------------------------> lineVector     
    |                     |        
	D---------------------C 
	
	*/

	const float lineWidthHalf = lineWidth / 2.0f;

	const glm::vec2 a = (lineWidthHalf * normal);
	const glm::vec2 d = (lineWidthHalf * -normal);

	const glm::vec2 b = lineVector + (lineWidthHalf * normal);
	const glm::vec2 c = lineVector + (lineWidthHalf * -normal);




	const std::array<glm::vec2, 6> rectangle{a, d, b, d, c, b};

	
	

	return rectangle;

}

std::array<glm::vec2, 6> GeometryGenerator::generateRectangle(const float width, const float height)
{

	//Anti-ClockWise Winding
	const glm::vec2 unitSquare[6] =
	{

		/*
		_______
		|    /
		|   /
		|  /
		| /
		|/
		*/

		{0,0},
		{1,1}, 
		{0,1},




		/*
	
		    /|
		   / |
		  /  |
		 /   |
		/    |
	    ------
		*/

		{0,0},
		{1,0},
		{1,1}
	};

	std::array<glm::vec2, 6> rectangle;
	for (size_t i = 0; i < 6; ++i)
	{
		rectangle[i] = { unitSquare[i].x * width, unitSquare[i].y * height };

	}

	return rectangle;

}






GeometryGenerator::ShapeTrapezium GeometryGenerator::generateTrapezium(const float pointA, const float pointB, const float width)
{
	
	
	/* 
	     B
	   . |
	 .   |   winding order : anti-clockwise
	A....|
	|    |
	|    |
	|    |
	------

	*/




	const float maxHeight = (pointA >= pointB) ? pointA : pointB;
	const float minHeight = (pointA <= pointB) ? pointA : pointB;


	/* 
	    ^
		|
   A\   |   /B
	 \  |  /
	  \ | /
	  B\|/A

	*/

	const float quadrant = (pointA <= pointB) ? 1.0 : -1.0f;
	const float lineThickness = 2.0;
	//there is issue like A > B or B >A
	std::array<glm::vec2, 6> slopedRectangleLine = generateLine(glm::vec2{ width, quadrant * (maxHeight-minHeight)}, lineThickness);

	if (quadrant < 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			slopedRectangleLine[i].y = slopedRectangleLine[i].y + (maxHeight - minHeight);
		}
	}

	const std::array<glm::vec2, 6> baseRectangle = generateRectangle(width, minHeight);




	//Calculation for middle triangle
	float h = maxHeight - minHeight;//what if its zero
	
	glm::vec2 a = { 0,0 };
	glm::vec2 b = { width,0 };

	if (abs(pointA) > abs(pointB))
	{
		a = { 0,h };
	}
	else
	{
		b = { h,width };
	}

	//Middle triangle
	std::array<glm::vec2, 3> triangle;

	triangle[0] = glm::vec2{ 0.0f, 0.0f };

	triangle[1] = b;
	triangle[2] = a;


	//Now correct offsets
	//for Middle Triangle
	for (int i = 0; i < 3; ++i)
	{
		triangle[i].y = triangle[i].y + minHeight;
	}

	//for Sloped Rectangle (Line)
	for (int i = 0; i < 6; ++i)
	{
		slopedRectangleLine[i].y = slopedRectangleLine[i].y + minHeight;
	}

	ShapeTrapezium shapeTrapezium;
	shapeTrapezium.baseRectangle = baseRectangle;
	shapeTrapezium.triangle = triangle;
	shapeTrapezium.slopedRectangleLine = slopedRectangleLine;

	return shapeTrapezium;


}






/*
GeometryGenerator::MeshData GeometryGenerator::getColouredTrapeziumWithOffset(const float pointA, const float pointB, const float width, const glm::vec3 startOffset, const glm::vec4& lineColour, const glm::vec4& baseColour)
{ /*      B
       . |
	 .   |   winding order : anti-clockwise
    A....|
	|    |
    |    |
    |    |
	------

	



	const float maxHeight = (pointA >= pointB) ? pointA : pointB;
	const float minHeight = (pointA <= pointB) ? pointA : pointB;


	/*  ^
	    |
   A\   |   /B
	 \  |  /
	  \ | /
	  B\|/A
	
	

	const float quadrant = (pointA <= pointB) ? 1.0 : -1.0f;
	const float lineThickness = 3;
	//there is issue like A > B or B >A 
    std::array<glm::vec2, 6> slopedRectangleLine = generateLine(glm::vec2{ width, quadrant * (maxHeight)}, lineThickness);



	const std::array<glm::vec2, 6> baseRectangle = generateRectangle(minHeight, width);




	//Calculation for middle triangle
	float h = maxHeight - minHeight;//what if its zero
	float w = 0;
	
	if (pointA > pointB)
	{
		w = 0;
	}
	else
	{
		w = width;
	}

	//Middle triangle
	glm::vec2 triangle[3];
	triangle[0] = glm::vec2{ 0.0f, 0.0f };
	triangle[1] = glm::vec2{ 0.0f, width };
	triangle[2] = glm::vec2{ h, w };


	//Now correct offsets
	//for Middle Triangle
	for (int i = 0; i < 3; ++i)
	{
		triangle[i].y = triangle[i].y + minHeight;
    }

	//for Sloped Rectangle (Line)
	for (int i = 0; i < 6; ++i)
	{
		slopedRectangleLine[i].y = slopedRectangleLine[i].y + minHeight;
	}



	//combine all

	std::array<glm::vec2, 15> trapezium;

	for (int i = 0; i < 6; ++i)
	{
		trapezium[i] = baseRectangle[i];
	}

	for (int i = 6; i < 6+3; ++i)//till 9
	{
		trapezium[i] = triangle[i-6];
	}

	for (int i = 9; i < 9 + 6; ++i)//till 15
	{
		trapezium[i] = slopedRectangleLine[i-9];
	}



	//return trapezium;
	return MeshData();

}
*/



