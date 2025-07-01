#pragma once

#include <memory>
#include <vector>
#include "core/vertex_input_layout.h"

namespace TheEngine::Graphics
{
	class GPUMesh : public IGPUResource// change to just Mesh or GPUMesh
	{
	private: 
		std::unique_ptr<IGPUResourceSet> m_resourceSet;

		std::vector<std::unique_ptr<IGPUBuffer>>m_vertexBuffers; 
		std::unique_ptr<IGPUBuffer> m_indexBuffer;
		
		bool isIndexed;

	private:
		VertexInputLayout m_vertexInputLayout;
		
	public:

		ResourceType getType() const override { return ResourceType::MESH; }

		IGPUResourceSet* getResourceSet() const;

		void setResourceSet(std::unique_ptr<IGPUResourceSet> resourceSet);
		void addVertexBuffer(std::unique_ptr<IGPUBuffer> vertexBuffer);
		void setVertexLayout(VertexInputLayout vertexInputLayout);
		~GPUMesh() override;
	};

}