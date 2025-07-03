#pragma once
#include "cpu_resource.h"
#include <vector>
#include "../../graphics/include/core/vertex_input_layout.h"



namespace TheEngine::Resource
{

	class CPUMesh : public CPUResource
	{
	private:

		std::vector < std::vector<uint8_t>> m_vertexBufferList;
		std::vector<uint8_t> m_indexBuffer;

	 const TheEngine::Graphics::VertexInputLayout* const m_vertexInputLayout;

		bool isIndexed;

	public:
		CPUMesh(const std::vector<std::vector<uint8_t>>&& vertexBufferList, const TheEngine::Graphics::VertexInputLayout* vertexInputLayout, bool isIndexed);
		CPUResourceType getType() const override;

	};
}