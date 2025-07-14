#include "../include/cpu_mesh.h"

namespace TheEngine::Resource
{

	CPUMesh::CPUMesh(const std::vector<std::vector<uint8_t>>&& vertexBufferList, 
		const TheEngine::Graphics::VertexInputLayout* vertexInputLayout,
		bool isIndexed) 
		: isIndexed(isIndexed), m_vertexBufferList(vertexBufferList),
		m_vertexInputLayout(vertexInputLayout)
	{
	}

	CPUResourceType CPUMesh::getType() const
	{
		return CPUResourceType::MESH;
	}

}