
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_buffer_transfer_manager.h>
#include <rendering-system/low-level-gpu-systems/gpu_buffer_manager.h>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_memory_system_data_types.h>


#include <assert.h>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/gpu_buffer_bump_suballocator.h>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/gpu_buffer_circular_suballocator.h>


namespace TheEngine::RenderingSystem
{

	/*
	void GPUMeshSystem::upload(GPUSubAllocationInfo& gpuSubAllocationInfo, GPUBufferInfo& targetBuffer, MeshUploadData&& meshUploadData, const TransferPriority& transferPriority)
	{


		m_gpuBufferTransferManager.transferDataToGPUBuffer(

			GPUBufferTransferRequest
			(
			std::move(meshUploadData.meshData),
			meshUploadData.meshDataSizeInBytes,
			targetBuffer,
			gpuSubAllocationInfo.offset,
			transferPriority
			)

		);
	}
	*/

	GPUMeshSystem::GPUMeshSystem(GPUBufferManager& gpuBufferManager, GPUBufferTransferManager& gpuBufferTransferManager) :
		m_gpuBufferManager(gpuBufferManager),
		m_gpuBufferTransferManager(gpuBufferTransferManager)
	{


	}

	std::unique_ptr<IGPUBufferSubAllocator> GPUMeshSystem::createAllocator(const GPUBufferInfo& GPUBufferInfo, const BufferUsage& bufferUsage)
	{


		switch (bufferUsage)
		{
		case BufferUsage::DYNAMIC:
		{
			//Circular , Linear, ...
			return std::make_unique<GPUBufferCircularSubAllocator>(GPUBufferInfo);
			break;
		}

		case BufferUsage::STREAM:
		{
			//circular, linear...
			return std::make_unique <GPUBufferCircularSubAllocator>(GPUBufferInfo);
			break;
		}

		case BufferUsage::STATIC:
		{
			return std::make_unique <GPUBufferBumpSubAllocator>(GPUBufferInfo);
			//tlsf , or general purpose allocator
			break;
		}

		default:
			assert(false && "Invalid BufferUsage");

			break;

		}


	}


	std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& GPUMeshSystem::getVertexMap(const BufferUsage bufferUsage)
	{
		switch (bufferUsage)
		{
		case BufferUsage::DYNAMIC:
		{
			return m_dynamicMeshMap;
			break;
		}

		case BufferUsage::STREAM:
		{
			return m_streamMeshMap;
			break;
		}

		case BufferUsage::STATIC:
		{
			return m_staticMeshMap;
			break;
		}

		default:
			assert(false && "Invalid BufferUsage");
			return m_staticMeshMap;
			break;

		}
	}


	IGPUBufferSubAllocator& GPUMeshSystem::getAllocatorForVertexFormat(std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap, const VertexFormat vertexFormat, const BufferUsage bufferUsage)
	{
		auto it = vertexMap.find(vertexFormat);



		if (it == vertexMap.end())
		{




			//need specific allocator

			const size_t totalBufferSizeNeeded = 1024 * 1024 * 200;//200 MB out to do it

			GPUBufferCreateInfo gpuBufferCreateInfo;
			gpuBufferCreateInfo.gpuBufferType = GPUBufferType::STORAGE;//SSBO
			gpuBufferCreateInfo.size = totalBufferSizeNeeded;
			gpuBufferCreateInfo.memoryFlags = MemoryFlags::COHERENT | MemoryFlags::CPU_VISIBLE | MemoryFlags::PERSISTENT_MAPPED;

			GPUBufferInfo allocatedGPUBufferInfo = m_gpuBufferManager.createBuffer(gpuBufferCreateInfo);

			vertexMap.emplace(vertexFormat, std::move(createAllocator(allocatedGPUBufferInfo, bufferUsage)));


			it = vertexMap.find(vertexFormat);
		}

		//TODO : There is an issue even if we get a allocator for that 
		// vertex format it could be full and that would require a 
		// copy to new buffer then delete old and then return this new buffer

		return *(it->second.get());

	}



	MeshInfo GPUMeshSystem::uploadMeshData(MeshUploadData&& meshUploadData)
	{

		//ToDo : Need to return MeshInfo not offset
		//TODO : How about we just use swtich statement here to get the maps and use that to use a acommon method to handle allocation etc and common logic
		//MeshInfo uploadMesh(std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& meshMap,std::map<IndexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& indexMap, MeshUploadData&& meshUploadData)
		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap = getVertexMap(meshUploadData.bufferUsage);
		IGPUBufferSubAllocator& gpuAllocator = getAllocatorForVertexFormat(vertexMap, meshUploadData.vertexFormat, meshUploadData.bufferUsage);


		//TODO : check size matches expected vertex format count 
		GPUSubAllocationInfo gpuSubAllocationInfo = gpuAllocator.allocate(meshUploadData.meshData.getSize());

		if (!gpuSubAllocationInfo.isAllocationSuccessful)
		{
			//Need to transfer all old data new buffer and create new allocator with 
			// old bookeeping data and also get new allocation also

			//OR use new buffer with new bookkepping

			//for now fail
			assert(false && "Failed To allocate memory for Mesh data");

		}


		MeshInfo meshInfo;

		GPUBufferTransferRequest request{
			std::move(meshUploadData.meshData),

			gpuAllocator.getGPUBufferInfo(),
			gpuSubAllocationInfo.offset,
			TransferPriority::HIGH
		};



		m_gpuBufferTransferManager.transferDataToGPUBuffer(std::move(request));


		meshInfo.bufferUsage = meshUploadData.bufferUsage;
		meshInfo.vertexDataSizeInBytes = gpuSubAllocationInfo.size;
		meshInfo.vertexFormat = meshUploadData.vertexFormat;
		meshInfo.vertexOffsetInBuffer = gpuSubAllocationInfo.offset;
		meshInfo.vertexCount = meshUploadData.vertexCount;



		if (meshUploadData.hasIndex)
		{
			meshInfo.hasIndex = true;
			//meshInfo.indexFormat = ;
			//meshInfo.indexDataSizeInBytes = ;
			//meshInfo.indexOffsetInBuffer = ;
			/*
			GPUBufferTransferRequest indexDataUpload
			{

			}


			m_gpuBufferTransferManager.transferDataToGPUBuffer();
			*/

			assert(false && "Code for Index Data Upload not yet written");
		}


		return meshInfo;
	}

	void GPUMeshSystem::deleteMeshDataFromGPU(const MeshId& meshId)
	{
		//Check if valid Id 

	}

	bool GPUMeshSystem::isAllUploadComplete() const
	{

		return m_gpuUploadComplete;

	}


	GPUBufferInfo GPUMeshSystem::getGPUBufferInfoForVetexFormat(const VertexFormat& vertexFormat, const BufferUsage& bufferUsage)
	{

		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap = getVertexMap(bufferUsage);
		IGPUBufferSubAllocator& gpuAllocator = getAllocatorForVertexFormat(vertexMap, vertexFormat, bufferUsage);//yeah this method also creates new buffer if not there



		return gpuAllocator.getGPUBufferInfo();

	}

}