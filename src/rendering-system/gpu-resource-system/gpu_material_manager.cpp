#pragma once
#include <assert.h>
#include "rendering-system/gpu-resource-system/gpu_material_manager.h"
#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>
#include <rendering-system/utils/gpu-allocators/gpu_buffer_fixed_size_suballocator.h>
#include <rendering-system/rhi/i_buffer_manager.h>
#include <rendering-system/rhi/i_transfer_manager.h>
#include <rendering-system/rhi/data-structures/gpu_buffer_data_types.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/rhi/i_texture_manager.h>


namespace TheEngine::RenderingSystem
{
	using namespace TheEngine::Memory;


	GPUMaterialManager::GPUMaterialManager(IBufferManager& bufferManager, ITransferManager& transferManager, ITextureManager& textureManager) :

		m_bufferManager(bufferManager),
		m_transferManager(transferManager),
		m_textureManager(textureManager)
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

			const size_t SIZE_OF_ONE_MATERIAL_IN_BYTES = memoryBlock.getSize();
			const size_t totalBufferSizeNeeded = SIZE_OF_ONE_MATERIAL_IN_BYTES * 200; //200 number of materials


			BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.bufferUsage = BufferUsage::STORAGE_BUFFER_BIT | BufferUsage::TRANSFER_DST_BIT;
			bufferCreateInfo.size = totalBufferSizeNeeded;
			bufferCreateInfo.memoryPropertyFlags = MemoryPropertyFlags::HOST_VISIBLE_BIT | MemoryPropertyFlags::HOST_COHERENT_BIT;

			BufferHandle allocatedBufferHandle = m_bufferManager.createBuffer(bufferCreateInfo);

			//Allocator needed is fixed size
			std::unique_ptr<IGPUBufferSubAllocator>  gpuBufferFixedSizeSubAllocator = std::make_unique<GPUBufferFixedSizeSubAllocator>(SIZE_OF_ONE_MATERIAL_IN_BYTES, allocatedBufferHandle, totalBufferSizeNeeded);
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

		BufferTransferRequest uploadRequest
		{
			std::move(memoryBlock),

			it->second->getBufferHandle(),
			gpuSubAllocationInfo.offset,
			TransferPriority::HIGH

		};


		m_transferManager.transferToBuffer(std::move(uploadRequest));




		return materialId;

	}

	uint64_t GPUMaterialManager::getBindlessTextureHandle(const TextureHandle& textureHandle) const
	{
		TextureMetadata textureMetadata = m_textureManager.getTextureMetadata(textureHandle);
		assert(textureMetadata.isBindless && "Texture is not bindless, cannot get bindless handle");
		return textureMetadata.bindlessHandle;
	}

	BufferHandle GPUMaterialManager::getBufferHandleForMaterial(const ShadingModel& shadingModel) const
	{


		auto it = m_shadingTypeToAllocators.find(shadingModel);

		if (it == m_shadingTypeToAllocators.end())
		{
			assert(false && "NO Buffer for Shading Model");
		}

		return it->second->getBufferHandle();

	}










}