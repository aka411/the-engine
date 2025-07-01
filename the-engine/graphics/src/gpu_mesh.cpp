#include "../include/gpu_mesh.h"



TheEngine::Graphics::IGPUResourceSet* TheEngine::Graphics::GPUMesh::getResourceSet() const
{
	return m_resourceSet.get();
}
void TheEngine::Graphics::GPUMesh::setResourceSet(std::unique_ptr<IGPUResourceSet> resourceSet)
{
	m_resourceSet = std::move(resourceSet);
}

void TheEngine::Graphics::GPUMesh::addVertexBuffer(std::unique_ptr<IGPUBuffer> vertexBuffer)
{
	m_vertexBuffers.push_back(std::move(vertexBuffer));
}

void TheEngine::Graphics::GPUMesh::setVertexLayout(VertexInputLayout vertexInputLayout)
{
	m_vertexInputLayout = vertexInputLayout;
}


TheEngine::Graphics::GPUMesh::~GPUMesh()
{
	// Cleanup if needed, unique_ptr will automatically handle resource deallocation
}