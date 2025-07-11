#include "../include/gpu_mesh.h"


namespace TheEngine::Graphics
{

	void TheEngine::Graphics::GPUMesh::addVertexBuffer(std::unique_ptr<IGPUBuffer>&& vertexBuffer)
	{
		m_vertexBuffers.push_back(std::move(vertexBuffer));
	}

	void TheEngine::Graphics::GPUMesh::setVertexLayout(VertexInputLayout vertexInputLayout)
	{
		m_vertexInputLayout = vertexInputLayout;
	}

	void GPUMesh::setIndexBuffer(std::unique_ptr<IGPUBuffer>&& indexBuffer)
	{
		m_indexBuffer = std::move(indexBuffer);
	}


	TheEngine::Graphics::GPUMesh::~GPUMesh()
	{
		// Cleanup if needed, unique_ptr will automatically handle resource deallocation
	}
}