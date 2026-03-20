#pragma once
#include <assert.h>

#include "rendering-system/gpu-resource-system/gpu_material_manager.h"
#include "rendering-system/low-level-gpu-systems/memory-management/gpu_buffer_transfer_manager.h"
#include <rendering-system/low-level-gpu-systems/gpu_buffer_manager.h>
#include <rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/gpu_buffer_fixed_size_suballocator.h>

namespace TheEngine::RenderingSystem
{



	GPUMaterialManager::GPUMaterialManager(GPUBufferManager& gpuBufferManager, GPUBufferTransferManager& gpuBufferTransferManager) :

		m_gpuBufferManager(gpuBufferManager),
		m_gpuBufferTransferManager(gpuBufferTransferManager)

	{


		//GPUPBRMaterial should be 80 bytes if its correctly padded for gpu buffer
		static_assert(sizeof(GPUPBRMaterial) == 80 && "Compiler has added padding in structs to be send to gpu, this will cause misalignment");

	}

	//Yeah this method moves data i better mark this as move
	MaterialId GPUMaterialManager::uploadMaterial(const ShadingModel shadingModel, MemoryBlock& memoryBlock)
	{

		auto it = m_shadingTypeToAllocators.find(shadingModel);

		if (it == m_shadingTypeToAllocators.end())
		{
			//not found
			//need to create a allocator for it
			//assert(false && "ShadingModel's allocator not found");

			//create allocator
			//and continue

			const size_t SIZE_OF_ONE_MATERIAL_IN_BYTES = memoryBlock.getSize();
			const size_t totalBufferSizeNeeded = SIZE_OF_ONE_MATERIAL_IN_BYTES * 200; //200 number of materials


			GPUBufferCreateInfo gpuBufferCreateInfo;
			gpuBufferCreateInfo.gpuBufferType = GPUBufferType::STORAGE;//SSBO
			gpuBufferCreateInfo.size = totalBufferSizeNeeded;
			gpuBufferCreateInfo.memoryFlags = MemoryFlags::COHERENT | MemoryFlags::CPU_VISIBLE | MemoryFlags::PERSISTENT_MAPPED;

			GPUBufferInfo allocatedGPUBufferInfo = m_gpuBufferManager.createBuffer(gpuBufferCreateInfo);

			//Allocator needed is fixed size
			std::unique_ptr<IGPUBufferSubAllocator>  gpuBufferFixedSizeSubAllocator = std::make_unique<GPUBufferFixedSizeSubAllocator>(SIZE_OF_ONE_MATERIAL_IN_BYTES,allocatedGPUBufferInfo);
			m_shadingTypeToAllocators[shadingModel] = std::move(gpuBufferFixedSizeSubAllocator);

			it = m_shadingTypeToAllocators.find(shadingModel);
		}

		//TODO : Assert size is as expected for the given shading Model
		GPUSubAllocationInfo  gpuSubAllocationInfo  = it->second.get()->allocate(memoryBlock.getSize());

		if (!gpuSubAllocationInfo.isAllocationSuccessful)
		{
			assert(false && "Failed to get space in material buffer to upload data");
		}


		//Material Id mapping is pain 
		MaterialId materialId = static_cast<MaterialId>(gpuSubAllocationInfo .offset/ memoryBlock.getSize());

		GPUBufferTransferRequest uploadRequest
		{
			std::move(memoryBlock),

			it->second.get()->getGPUBufferInfo(),
			gpuSubAllocationInfo.offset,
			TransferPriority::HIGH

		};


		m_gpuBufferTransferManager.transferDataToGPUBuffer(std::move(uploadRequest));




		return materialId;

	}



	GPUBufferInfo GPUMaterialManager::getGPUBufferInfoForMaterial(const ShadingModel& shadingModel)
	{


		auto it = m_shadingTypeToAllocators.find(shadingModel);

		if (it == m_shadingTypeToAllocators.end())
		{
			assert(false && "NO Buffer for Shading Model");
		}

		return it->second->getGPUBufferInfo();

	}





	bool GPUMaterialManager::isAllUploadComplete() const
	{
		return m_gpuUploadComplete;

	}




}