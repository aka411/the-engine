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




	const float maxHeight = std::max(pointA , pointB);
	const float minHeight = std::min(pointA, pointB);


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
	std::array<glm::vec2, 6> slopedRectangleLine = generateLine(glm::vec2{ width,quadrant*(maxHeight - minHeight)}, lineThickness);

	if (quadrant < 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			slopedRectangleLine[i].y = slopedRectangleLine[i].y + (maxHeight - minHeight);
		}
	}

	const std::array<glm::vec2, 6> baseRectangle = generateRectangle(width, minHeight);




	//Calculation for middle triangle
	const float h = maxHeight- minHeight;//what if its zero
	
	glm::vec2 a = { 0, 0 };          // Bottom Left (local to the top section)
	glm::vec2 b = { width, 0 };      // Bottom Right
	glm::vec2 c = (pointA < pointB) ? glm::vec2(width, h) : glm::vec2(0, h);


	//Middle triangle

	/* 
		 |
		 |
	  --a|-------b------>x
		 |
		 |

	*/

	std::array<glm::vec2, 3> triangle;

	triangle[0] = a;
	triangle[1] = b;
	triangle[2] = c;






	//APPLYING OFFSETS 

	
	//for Middle Triangle

	for (int i = 0; i < 3; ++i)
	{
		triangle[i].y = triangle[i].y + minHeight;
	}


	//This needs more fine tuning
	//for Sloped Rectangle (Line)
	for (int i = 0; i < 6; ++i)
	{
		slopedRectangleLine[i].y = slopedRectangleLine[i].y + minHeight ;
	}

	ShapeTrapezium shapeTrapezium;
	shapeTrapezium.baseRectangle = baseRectangle;
	shapeTrapezium.triangle = triangle;
	shapeTrapezium.slopedRectangleLine = slopedRectangleLine;

	return shapeTrapezium;


}



