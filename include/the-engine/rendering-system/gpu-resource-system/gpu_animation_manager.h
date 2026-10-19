#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>


namespace TheEngine::RenderingSystem
{
	class IBufferManager;
	class ITransferManager;

	class IGPUBufferSubAllocator;

	using BoneJointMatrixSetId = uint32_t;


	//Might need to rename to name related to skin animation or something
	//GPUSkeletonBufferManager or GPUSkinningManager
	class GPUAnimationManager
	{

	private:



		IBufferManager& m_bufferManager;
		ITransferManager& m_transferManager;

		std::unique_ptr<IGPUBufferSubAllocator> m_JointMatrixSetAllocator;

	public:

		GPUAnimationManager(IBufferManager& bufferManager, ITransferManager& transferManager);

		~GPUAnimationManager();

		BoneJointMatrixSetId uploadNewJointMatrixSetToGPUBuffer(std::vector<glm::mat4>&& jointMatrixSet);//One time registration method
		void updateJointMatrixSetInGPUBuffer(const BoneJointMatrixSetId& boneJointMatrixSetId, std::vector<glm::mat4>&& jointMatrixSet);


		void releaseJointMatrixSet(BoneJointMatrixSetId id);

	};




}