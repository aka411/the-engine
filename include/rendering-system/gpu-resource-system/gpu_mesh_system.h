#pragma once
#include <map>
#include <vector>
#include <memory>

#include <rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h>
#include <rendering-system/engine_handles.h>
#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>
#include <rendering-system/rhi/i_transfer_manager.h>





namespace TheEngine::RenderingSystem
{




	class IBufferManager;
	class ITransferManager;

	class IGPUBufferSubAllocator;

	class GPUMeshSystem
	{

	private:

		

	



		IBufferManager& m_bufferManager;
		ITransferManager& m_transferManager;


		//Vertex Format to Buffer

		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>> m_staticMeshMap;
		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>> m_dynamicMeshMap;
		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>> m_streamMeshMap;

		//MeshId from here
		std::vector<MeshInfo> m_meshInfos;//do i even need this


		//For Index Buffer

		std::map<IndexFormat, std::unique_ptr<IGPUBufferSubAllocator>> m_staticIndexMap;
		std::map<IndexFormat, std::unique_ptr<IGPUBufferSubAllocator>> m_dynamicIndexMap;
		std::map<IndexFormat, std::unique_ptr<IGPUBufferSubAllocator>> m_streamIndexMap;




		bool m_gpuUploadComplete = true;

		//helper to upload

		void upload(GPUSubAllocationInfo& gpuSubAllocationInfo, BufferHandle& targetBufferHandle, MeshUploadData&& meshUploadData, const TransferPriority& transferPriority);
		
		std::unique_ptr<IGPUBufferSubAllocator> createAllocator(const BufferHandle& bufferHandle, const size_t bufferSize, const BufferResourceUsageHint& bufferResourceUsageHint);

		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& getVertexMap(const BufferResourceUsageHint& bufferResourceUsageHint);
		IGPUBufferSubAllocator& getAllocatorForVertexFormat(std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap, const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint);


	public:

		GPUMeshSystem(IBufferManager& bufferManager, ITransferManager& transferManager);

		MeshInfo uploadMeshData(MeshUploadData&& meshUploadData);

		void deleteMeshDataFromGPU(const MeshId& meshId);


		bool isAllUploadComplete() const;


		//For giving out the buffers
		const BufferHandle getBufferHandleForVertexFormat(const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint);





	};


}