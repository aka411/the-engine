#pragma once
#include <rendering-system/engine_handles.h>
#include <rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h>
#include <rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h>

namespace TheEngine::RenderingSystem
{

	class GPUResourceSystem;

	class GPUResourceResolver
	{

	private:

		GPUResourceSystem& m_gpuResourceSystem;

	public:

		GPUResourceResolver(GPUResourceSystem& gpuResourceSystem);

		const BufferHandle getMeshBufferHandle(const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint) const;
		const BufferHandle getIndexBufferhandle(const IndexFormat& indexFormat, const BufferResourceUsageHint& bufferResourceUsageHint) const;
		const BufferHandle getMaterialBufferHandle(const ShadingModel& shadingModel) const;


		//const BufferHandle getAnimationBufferHandle() const;

	};



}