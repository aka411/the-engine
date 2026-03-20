#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/i_gpu_buffer_suballocator.h>
#include <memory>

namespace TheEngine::RenderingSystem
{
	class GPUBufferManager;
	class GPUBufferTransferManager;

	using BoneJointMatrixSetId = uint32_t;


	//Might need to rename to name related to skin animation or something
	//GPUSkeletonBufferManager or GPUSkinningManager
	class GPUAnimationManager
	{

	private:



		GPUBufferManager& m_gpuBufferManager;
		GPUBufferTransferManager& m_gpuBufferTransferManager;

		std::unique_ptr<IGPUBufferSubAllocator> m_boneMatricesAllocator;

	public:

		GPUAnimationManager(GPUBufferManager& gpuBufferManager, GPUBufferTransferManager& gpuBufferTransferManager);


		BoneJointMatrixSetId uploadNewJointMatrixSetToGPUBuffer(std::vector<glm::mat4>&& jointMatrixSet);//One time registration method
		void updateJointMatrixSetInGPUBuffer(const BoneJointMatrixSetId& boneJointMatrixSetId, std::vector<glm::mat4>&& jointMatrixSet);


		void releaseJointMatrixSet(BoneJointMatrixSetId id);

	};




}