#pragma once 
#include "glad/glad.h"

namespace TheEngine::RenderingSystem
{




	/* BUFFER SYSTEM */
	enum class BufferUsageType
	{
		UNKNOWN,
		STATIC,
		DYNAMIC,
		STREAMING

	};

	enum BufferType
	{
		VERTEX,
		INDEX,
		SSBO,
		UBO,
		UNKNOWN
	};



	//This is the equivalent of pointer in normal ram memory allocation
	struct GPUSubAllocationInfo
	{
		size_t offset = 0;
		size_t size = 0; //redundant though
		bool isValid = false;

	};




	struct GPUBufferInfo
	{
		BufferType bufferType = BufferType::UNKNOWN;

		GLuint bufferHandle;

		//size in bytes
		size_t size = 0;

		void* mappedPtr = nullptr;

		bool isMapped = false;

		//other flags maybe

	};



}