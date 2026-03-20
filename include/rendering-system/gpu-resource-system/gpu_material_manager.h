#pragma once
#include <map>
#include <memory>
#include <cstddef>
#include "rendering-system/low-level-gpu-systems/memory-management/memory_system_data_types.h"
#include "rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h"
#include "rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/i_gpu_buffer_suballocator.h"
#include <memory-management/memory_data_types.h>


namespace TheEngine::RenderingSystem
{


	class GPUBufferManager;
	class GPUBufferTransferManager;

	class GPUMaterialManager
	{
	private:



		GPUBufferManager& m_gpuBufferManager;

		GPUBufferTransferManager& m_gpuBufferTransferManager;


		//TODO : use fixed size allocators 

		std::map<ShadingModel, std::unique_ptr<IGPUBufferSubAllocator>> m_shadingTypeToAllocators;


		bool m_gpuUploadComplete = true;


		MaterialId uploadMaterial(const ShadingModel shadingModel, MemoryBlock& memoryBlock);
		

	public:

		GPUMaterialManager(GPUBufferManager& gpuBufferManager, GPUBufferTransferManager& gpuBufferTransferManager);


		//One method to handle all material types or multiple methods for different materials
		//TODO : This method is not correct casue there is no separation of data representation,or may be use this itself
	


		template<typename T>
		MaterialId uploadStruct(const ShadingModel model, const T& structData) {

			MemoryBlock memoryBlock(reinterpret_cast<const std::byte*>(&structData), sizeof(T));

			return uploadMaterial(model, memoryBlock);
		};





		/*-----For Rendering System-------*/

		//TODO : we also need to express no buffer also?
		GPUBufferInfo getGPUBufferInfoForMaterial(const ShadingModel& shadingModel);
		bool isAllUploadComplete() const;//maybe not needed??

	};

}