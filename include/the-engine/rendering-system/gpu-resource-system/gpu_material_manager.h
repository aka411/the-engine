#pragma once
#include <map>
#include <memory>
#include <cstddef>
#include <rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h>

#include <memory-management/memory_block.h>
#include <rendering-system/engine_handles.h>



namespace TheEngine::RenderingSystem
{


	class IBufferManager;
	class ITransferManager;
	class ITextureManager;

	class IGPUBufferSubAllocator;

	class GPUMaterialManager
	{
	private:



		IBufferManager& m_bufferManager;

		ITransferManager& m_transferManager;

		ITextureManager& m_textureManager;



		std::map<ShadingModel, std::unique_ptr<IGPUBufferSubAllocator>> m_shadingTypeToAllocators;


		bool m_gpuUploadComplete = true;


		MaterialId uploadMaterial(const ShadingModel shadingModel, TheEngine::Memory::MemoryBlock& memoryBlock);
		

	public:

		GPUMaterialManager(IBufferManager& bufferManager, ITransferManager& transferManager, ITextureManager& textureManager);


		//One method to handle all material types or multiple methods for different materials
		//TODO : This method is not correct casue there is no separation of data representation,or may be use this itself
	


		template<typename T>
		MaterialId uploadStruct(const ShadingModel model, const T& structData) {

			TheEngine::Memory::MemoryBlock memoryBlock(reinterpret_cast<const std::byte*>(&structData), sizeof(T));

			return uploadMaterial(model, memoryBlock);
		};



		uint64_t getBindlessTextureHandle(const TextureHandle& textureHandle) const;
	

		/*-----For Rendering System-------*/

		//TODO : we also need to express no buffer also?
		BufferHandle getBufferHandleForMaterial(const ShadingModel& shadingModel) const;
		//bool isAllUploadComplete() const;//maybe not needed??

	};

}