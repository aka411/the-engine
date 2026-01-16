#pragma once
#include <vector>
#include <glm/glm.hpp>


class UIGeometryGenerator
{
private:


public:

	//need more thought here
	struct MeshData
	{
		//I think i need to add format info here too

		size_t numOfVertex = 0;
		std::vector<std::byte> data;
	};
	
	static MeshData generate2DLine(const glm::vec2& pointA, const glm::vec2& pointB, const float lineWidth, const glm::vec4& colour);
	static MeshData generateRectangle(const float width, const float height,  const glm::vec3& offset, const glm::vec4& colour);


	static MeshData generateShadedLineGraph(
		const float pointA,
		const float pointB,
		const float width,
		const glm::vec3& startOffset,
		const glm::vec4& lineColour,
		const glm::vec4& baseColour);




};