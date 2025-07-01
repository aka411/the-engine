#pragma once
#include "cpu_resource.h"
#include <vector>
#include "../../graphics/include/core/vertex_input_layout.h"


namespace TheEngine::Resource
{

	class CPUMesh : public CPUResource
	{
	public:
		std::vector<std::vector<float>> vertexBuffer;
		std::vector<std::vector<int>> indexBufferHandle;
		VertexInputLayout vertexInputLayout;
		bool isIndexed;

	public:
		CPUResourceType getType() const override { return CPUResourceType::MESH; }
	};
}