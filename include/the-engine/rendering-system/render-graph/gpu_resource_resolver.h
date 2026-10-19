#pragma once
#include <rendering-system/engine_handles.h>
#include <rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h>
#include <rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h>
#include <rendering-system/render-graph/render_graph_types.h>


namespace TheEngine::RenderingSystem
{

	class GPUResourceSystem;

	class GPUResourceResolver
	{

	private:

		GPUResourceSystem& m_gpuResourceSystem;
		RenderGraphResources& m_renderGraphResources;

	public:

		GPUResourceResolver(GPUResourceSystem& gpuResourceSystem, RenderGraphResources& renderGraphResources);

		const BufferHandle getMeshBufferHandle(const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint) const;
		const BufferHandle getIndexBufferhandle(const IndexFormat& indexFormat, const BufferResourceUsageHint& bufferResourceUsageHint) const;
		const BufferHandle getMaterialBufferHandle(const ShadingModel& shadingModel) const;


		//const BufferHandle getAnimationBufferHandle() const;


		//Texture
		const TextureHandle getTextureHandle(const std::string& name) const;
		const TextureHandle getTextureHandle(const VirtualTextureId virtualTextureId) const;
	};



}