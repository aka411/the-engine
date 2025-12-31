#pragma once
#include <map>
#include "ui/core/ui_core_data_structures.h"
#include "low-level/gpu_buffer_sub_bump_allocator.h"
#include "low-level/gpu_buffer_manager.h"


namespace TheEngine::UI
{


	class UIVertexBufferManagementSystem
	{

	private:

		GPUBufferManager& m_gpuBufferManager;

	
		std::map<UIVertexFormat,std::unordered_map<UIBufferType, GPUBufferSubBumpAllocator>> m_formatToNormalVertexBufferSubAllocators;
		std::map<UIVertexFormat, std::unordered_map<UIBufferType, GPUBufferSubBumpAllocator>> m_formatToTextVertexBufferSubAllocators;

		//std::map<UIBufferType, GPUBufferSubBumpAllocator> m_textVertexBufferAllocators;


		GPUBufferSubBumpAllocator createNewSSBOAllocator();
		GPUBufferSubBumpAllocator createNewVertexBufferAllocator();

	public:

		UIVertexBufferManagementSystem(GPUBufferManager& gpuBufferManager);

		//get buffer for given vertex format
		size_t uploadVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size);
		size_t uploadTextVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size);


		GPUBufferInfo getBufferInfoForVertexFormat(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType);
		GPUBufferInfo getBufferInfoForTextWithVertexFormat(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType);


	};


}