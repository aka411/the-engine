#pragma once

#include <memory>
#include <vector>
#include "core/vertex_input_layout.h"
#include "igpu_resource.h"
#include "igpu_buffer.h"

namespace TheEngine::Graphics
{
	class GPUMesh : public IGPUResource// change to just Mesh or GPUMesh
	{
	private: 
	

		std::vector<std::unique_ptr<IGPUBuffer>> m_vertexBuffers; 
		std::unique_ptr<IGPUBuffer> m_indexBuffer;
		
		bool isIndexed;

	private:
		VertexInputLayout m_vertexInputLayout;
		
	public:

		ResourceType getType() const override { return ResourceType::MESH; }

		
		void addVertexBuffer(std::unique_ptr<IGPUBuffer>&& vertexBuffer);
		void setVertexLayout(VertexInputLayout vertexInputLayout);


		void setIndexBuffer(std::unique_ptr<IGPUBuffer>&& indexBuffer);

		//add method maybe to insert whole vector

		~GPUMesh() override;
	};

}