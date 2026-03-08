#include "ui/utils/ui_geometry_generator.h"
#include <ui/utils/geometry_generator.h>



UIGeometryGenerator::MeshData UIGeometryGenerator::generate2DLine(const glm::vec2& pointA, const glm::vec2& pointB, const float lineWidth, const glm::vec4& colour)
{

	




	return MeshData();

}




UIGeometryGenerator::MeshData UIGeometryGenerator::generateRectangle(const float width, const float height, const glm::vec3& offset, const glm::vec4& colour)
{







	std::array<glm::vec2, 6> rectangle = GeometryGenerator::generateRectangle(width, height);



	MeshData meshData;
	meshData.numOfVertex = 6;
	meshData.data.resize((sizeof(glm::vec4) + sizeof(glm::vec4)) * 6);
	

	std::array<glm::vec4, 12> colouredRectangle;
	
	for (int i = 0 ; i < 6 ; ++i)
	{
		colouredRectangle[2 * i] =glm::vec4( rectangle[i].x + offset.x, rectangle[i].y + offset.y, 0.0 + offset.z, 1.0);
		colouredRectangle[(2*i) +1] = colour;
	}
	memcpy(&meshData.data[0], &colouredRectangle[0], sizeof(glm::vec4) * 12);
	return meshData;
}


UIGeometryGenerator::MeshData UIGeometryGenerator::generateShadedLineGraph(
	const float pointA,
	const float pointB,
	const float width,
	const glm::vec3& startOffset,
	const glm::vec4& lineColour,
	const glm::vec4& baseColour,
	bool yAxisUp

)
{

	glm::vec3 modifiedStartOffset = startOffset;

	if (!yAxisUp)
	{
		modifiedStartOffset.y = modifiedStartOffset.y * -1.0f;
	}

	const float yFlipFactor = (yAxisUp)? 1.0f : -1.0f;

	GeometryGenerator::ShapeTrapezium shapeTrapezium = GeometryGenerator::generateTrapezium(pointA, pointB, width);

	MeshData meshData;


	meshData.numOfVertex = 6 + 3 + 6;
	meshData.data.resize(sizeof(glm::vec4) * (6 + 3 + 6) * 2);


	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 colour;
	};

	Vertex* vertex = reinterpret_cast<Vertex*>(&meshData.data[0]);

	//LINE
	for (int i = 0; i < 6; ++i)
	{
		vertex[i].position= glm::vec4(shapeTrapezium.slopedRectangleLine[i].x + modifiedStartOffset.x, yFlipFactor*( shapeTrapezium.slopedRectangleLine[i].y + modifiedStartOffset.y), 0.0 + modifiedStartOffset.z, 1.0);
		vertex[i].colour = lineColour;
	}

	//Triangle Cap
	for (int i = 0; i < 3; ++i)
	{
		vertex[i + 6].position = glm::vec4(shapeTrapezium.triangle[i].x + modifiedStartOffset.x, yFlipFactor * (shapeTrapezium.triangle[i].y + modifiedStartOffset.y), 0.0 + modifiedStartOffset.z, 1.0);
		vertex[i + 6].colour = baseColour;

	}

	//Base Rectangle (Area Under Line)
	for (int i = 0; i < 6; ++i)
	{
		vertex[i + 9].position = glm::vec4(shapeTrapezium.baseRectangle[i].x + modifiedStartOffset.x, yFlipFactor * (shapeTrapezium.baseRectangle[i].y + modifiedStartOffset.y), 0.0 + modifiedStartOffset.z, 1.0);
		vertex[i + 9].colour = baseColour;


	}


	return meshData;

}