#include <assert.h>
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>

#include <rendering-system/utils/gpu-allocators/gpu_buffer_circular_suballocator.h>
#include <rendering-system/utils/gpu-allocators/gpu_buffer_bump_suballocator.h>
#include <rendering-system/rhi/i_buffer_manager.h>
#include <rendering-system/rhi/i_transfer_manager.h>
#include <rendering-system/rhi/data-structures/gpu_buffer_data_types.h>

namespace TheEngine::RenderingSystem
{



	GPUMeshSystem::GPUMeshSystem(IBufferManager& bufferManager, ITransferManager& transferManager) :
		m_bufferManager(bufferManager),
		m_transferManager(transferManager)
	{


	}

	std::unique_ptr<IGPUBufferSubAllocator> GPUMeshSystem::createAllocator(const BufferHandle& bufferHandle, const size_t bufferSize, const BufferResourceUsageHint& bufferResourceUsageHint)
	{


		switch (bufferResourceUsageHint)
		{
		case BufferResourceUsageHint::DYNAMIC:
		{
			//Circular , Linear, ...
			return std::make_unique<GPUBufferCircularSubAllocator>(bufferHandle, bufferSize);
			break;
		}

		case BufferResourceUsageHint::STREAM:
		{
			//circular, linear...
			return std::make_unique <GPUBufferCircularSubAllocator>(bufferHandle, bufferSize);
			break;
		}

		case BufferResourceUsageHint::STATIC:
		{
			return std::make_unique <GPUBufferBumpSubAllocator>(bufferHandle, bufferSize);
			//tlsf , or general purpose allocator
			break;
		}

		default:
			assert(false && "Invalid BufferUsage");

			break;

		}


	}


	 std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& GPUMeshSystem::getVertexMap(const BufferResourceUsageHint& bufferResourceUsageHint)
	{
		switch (bufferResourceUsageHint)
		{
		case BufferResourceUsageHint::DYNAMIC:
		{
			return m_dynamicMeshMap;
			break;
		}

		case BufferResourceUsageHint::STREAM:
		{
			return m_streamMeshMap;
			break;
		}

		case BufferResourceUsageHint::STATIC:
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


	IGPUBufferSubAllocator& GPUMeshSystem::getAllocatorForVertexFormat(std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap, const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint)
	{
		auto it = vertexMap.find(vertexFormat);



		if (it == vertexMap.end())
		{




			//need specific allocator

			const size_t totalBufferSizeNeeded = 1024 * 1024 * 200;//200 MB out to do it

			BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.bufferUsage = BufferUsage::TRANSFER_DST_BIT | BufferUsage::VERTEX_BUFFER_BIT;
			bufferCreateInfo.size = totalBufferSizeNeeded;
			bufferCreateInfo.memoryPropertyFlags = MemoryPropertyFlags::DEVICE_LOCAL_BIT;

			BufferHandle allocatedBufferHandle= m_bufferManager.createBuffer(bufferCreateInfo);

			vertexMap.emplace(vertexFormat, std::move(createAllocator(allocatedBufferHandle, totalBufferSizeNeeded, bufferResourceUsageHint)));


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
		 std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap = getVertexMap(meshUploadData.bufferResourceUsageHint);
		 IGPUBufferSubAllocator& gpuAllocator = getAllocatorForVertexFormat(vertexMap, meshUploadData.vertexFormat, meshUploadData.bufferResourceUsageHint);


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

		BufferTransferRequest request{
			std::move(meshUploadData.meshData),

			gpuAllocator.getBufferHandle(),
			gpuSubAllocationInfo.offset,
			TransferPriority::HIGH
		};



		m_transferManager.transferToBuffer(std::move(request));


		meshInfo.bufferResourceUsageHint = meshUploadData.bufferResourceUsageHint;
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


	const BufferHandle GPUMeshSystem::getBufferHandleForVertexFormat(const VertexFormat& vertexFormat, const BufferResourceUsageHint& bufferResourceUsageHint)
	{

		std::map<VertexFormat, std::unique_ptr<IGPUBufferSubAllocator>>& vertexMap = getVertexMap(bufferResourceUsageHint);
	    IGPUBufferSubAllocator& gpuAllocator = getAllocatorForVertexFormat(vertexMap, vertexFormat, bufferResourceUsageHint);//yeah this method also creates new buffer if not there



		return gpuAllocator.getBufferHandle();

	}

}