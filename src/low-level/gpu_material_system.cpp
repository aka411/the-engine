#include "low-level/gpu_material_system.h"
#include <cstring>

namespace TheEngine
{



	GPUMaterialSystem::GPUMaterialSystem(Memory::GPUBufferManager& gpuBufferManager) :
		m_gpuBufferManager(gpuBufferManager)
	{
		//create PBR SSBO
		Memory::GPUBufferInfo gpuBufferInfo = m_gpuBufferManager.createMappedSSBO(10 * 1024 * 1024, nullptr); //10 MiB

		Memory::GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);
		m_materialTypeToSSBO.emplace(
			MaterialType::PBR_METALLIC_ROUGHNESS,
			std::move(gpuBufferSubBumpAllocator));
	}

	MaterialId GPUMaterialSystem::uploadMaterial(MaterialType materialType, std::byte* ptr, size_t size) //size in bytes
	{


		//upload the and  give out material id
		Memory::GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_materialTypeToSSBO.at(materialType);


		Memory::AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
		Memory::GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();
		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		assert(bufferBasePtr != nullptr);


		//I have read that this is ok
		//                                std::byte* + size_t
		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;


		assert(gpuBufferInfo.size > allocationInfo.offset + size);


		memcpy(absoluteBufferPtr, ptr, size);


		return m_currentMaterialId++;
	}


	const Memory::GPUBufferInfo GPUMaterialSystem::getGPUBufferInfo(MaterialType materialType)
	{


		Memory::GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_materialTypeToSSBO.at(materialType);

		return gpuBufferSubBumpAllocator.getGPUBufferInfo();
	}


}