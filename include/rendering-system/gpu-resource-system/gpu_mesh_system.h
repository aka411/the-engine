#pragma once
#include <map>
#include <vector>
#include <memory.h>
#include "rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h"
#include "rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/i_gpu_buffer_suballocator.h"
#include "rendering-system/low-level-gpu-systems/memory-management/gpu_buffer_transfer_manager.h" // only for Transfer Priority enum




namespace TheEngine::RenderingSystem
{




	class GPUBufferManager;
	class GPUBufferTransferManager;

	class GPUMeshSystem
	{

	private:

		

		//lass IGPUBufferSubAllocator;



		GPUBufferManager& m_gpuBufferManager;
		GPUBufferTransferManager& m_gpuBufferTransferManager;


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
		void upload(GPUSubAllocationInfo& gpuSubAllocationInfo, GPUBufferInfo& targetBuffer, MeshUploadData&& meshUploadData, const TransferPriority& transferPriority);
		
		std::unique_ptr<IGPUBufferSubAllocator> createAllocator(const GPUBufferInfo& GPUBufferInfo, const BufferUsage& bufferUsage);
		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& getVertexMap(const BufferUsage bufferUsage);
		IGPUBufferSubAllocator& getAllocatorForVertexFormat(std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap, const VertexFormat vertexFormat, const BufferUsage bufferUsage);


	public:

		GPUMeshSystem(GPUBufferManager& gpuBufferManager, GPUBufferTransferManager& gpuBufferTransferManager);

		MeshInfo uploadMeshData(MeshUploadData&& meshUploadData);

		void deleteMeshDataFromGPU(const MeshId& meshId);


		bool isAllUploadComplete() const;


		//For giving out the buffers
		GPUBufferInfo getGPUBufferInfoForVetexFormat(const VertexFormat& vertexFormat, const BufferUsage& bufferUsage);





	};


}