#include <rendering-system/gpu-resource-system/gpu_animation_manager.h>
#include <rendering-system/utils/gpu-allocators/gpu_buffer_tlsf_suballocator.h>
#include <rendering-system/rhi/i_buffer_manager.h>
#include <rendering-system/rhi/i_transfer_manager.h>

namespace TheEngine::RenderingSystem
{


	GPUAnimationManager::GPUAnimationManager(IBufferManager& bufferManager, ITransferManager& transferManager) :

		m_bufferManager(bufferManager),
		m_transferManager(transferManager)
	{


		const size_t totalBufferSize = 1024 * 1024 * 100;//100 MB out to do it

		BufferCreateInfo bufferCreateInfo;
		bufferCreateInfo.bufferUsage = BufferUsage::TRANSFER_DST_BIT | BufferUsage::STORAGE_BUFFER_BIT;
		bufferCreateInfo.size = totalBufferSize;
		bufferCreateInfo.memoryPropertyFlags = MemoryPropertyFlags::HOST_VISIBLE_BIT| MemoryPropertyFlags::HOST_COHERENT_BIT| MemoryPropertyFlags::HOST_CACHED_BIT;


		BufferHandle bufferHandle = m_bufferManager.createBuffer(bufferCreateInfo);

		//create allocator
		m_JointMatrixSetAllocator = std::make_unique<GPUBufferTLSFSubAllocator>(bufferHandle, totalBufferSize);



	}


	GPUAnimationManager::~GPUAnimationManager()
	{

	}

	BoneJointMatrixSetId GPUAnimationManager::uploadNewJointMatrixSetToGPUBuffer(std::vector<glm::mat4>&& jointMatrixSet)
	{
		const size_t& sizeOfSetInBytes = jointMatrixSet.size() * sizeof(jointMatrixSet[0]);

		GPUSubAllocationInfo gpuSubAllocationInfo  = m_JointMatrixSetAllocator->allocate(sizeOfSetInBytes);


		assert(gpuSubAllocationInfo.isAllocationSuccessful && "GPUBufferSubAllocation failed");


		m_transferManager.quickTransfer(reinterpret_cast<std::byte*>(jointMatrixSet.data()), sizeOfSetInBytes, m_JointMatrixSetAllocator->getBufferHandle(), gpuSubAllocationInfo.offset);


		return gpuSubAllocationInfo.offset;
	}
	

	void GPUAnimationManager::updateJointMatrixSetInGPUBuffer(const BoneJointMatrixSetId& boneJointMatrixSetId, std::vector<glm::mat4>&& jointMatrixSet)
	{





	}


	void GPUAnimationManager::releaseJointMatrixSet(BoneJointMatrixSetId id)
	{


	}


}