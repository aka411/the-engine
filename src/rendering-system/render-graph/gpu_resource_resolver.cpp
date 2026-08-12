#include <rendering-system/render-graph/gpu_resource_resolver.h>
#include <rendering-system/gpu-resource-system/gpu_resource_system.h>
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>
#include <rendering-system/gpu-resource-system/gpu_material_manager.h>

namespace TheEngine::RenderingSystem
{



	GPUResourceResolver::GPUResourceResolver(GPUResourceSystem& gpuResourceSystem, RenderGraphResources& renderGraphResources):
		m_gpuResourceSystem(gpuResourceSystem),
		m_renderGraphResources(renderGraphResources)
	{

	}

	const BufferHandle GPUResourceResolver::GPUResourceResolver::getMeshBufferHandle(const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint) const
	{
		return m_gpuResourceSystem.getGPUMeshSystem().getBufferHandleForVertexFormat(vertexFormat, bufferResourceUsageHint);
	}

	const BufferHandle GPUResourceResolver::getIndexBufferhandle(const IndexFormat& indexFormat, const BufferResourceUsageHint& bufferResourceUsageHint) const
	{
		return m_gpuResourceSystem.getGPUMeshSystem().getBufferHandleForIndexFormat(indexFormat, bufferResourceUsageHint);
	}

	const BufferHandle GPUResourceResolver::getMaterialBufferHandle(const ShadingModel& shadingModel) const
	{
		return m_gpuResourceSystem.getGPUMaterialManager().getBufferHandleForMaterial(shadingModel);
	}

/*
	const BufferHandle GPUResourceResolver::getAnimationBufferHandle() const
	{

	}
*/

	const TextureHandle GPUResourceResolver::getTextureHandle(const std::string& name) const
	{
		return m_renderGraphResources.getTextureHandle(name);
	}

	const TextureHandle GPUResourceResolver::getTextureHandle(const VirtualTextureId virtualTextureId) const
	{
		return m_renderGraphResources.getTextureHandle(virtualTextureId);
	}



}