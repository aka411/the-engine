#pragma once
#include <assert.h>
#include <cstdint>
#include <memory-management/memory_block.h>


namespace TheEngine::RenderingSystem
{


	using MeshOffset = size_t;

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC,
		STREAM
	};

	//  Vertex Formats //

	enum class VertexFormat
	{
		STANDARD,// - Position + Normal + UV
		STANDARD_SKINNED,// - Position + Normal + UV + Bone Weights + Bone Indices

	};

	enum class IndexFormat
	{
		UINT16,
		UINT32
	};




	struct MeshUploadData
	{
		VertexFormat vertexFormat;
		BufferUsage bufferUsage;

		TheEngine::Memory::MemoryBlock meshData;
		//size_t meshDataSizeInBytes = 0;
		size_t vertexCount;


		bool hasIndex = false;

		IndexFormat indexFormat;
		TheEngine::Memory::MemoryBlock indexData;
		size_t indexCount;
		//size_t indexDataSizeInBytes = 0;



	};


	//How about a mesh Id

	using MeshId = uint64_t;

	struct MeshInfo
	{
		MeshId meshId;
		VertexFormat vertexFormat;
		BufferUsage bufferUsage;

		bool hasIndex;
		IndexFormat indexFormat;

		size_t vertexDataSizeInBytes;
		size_t indexDataSizeInBytes;

		size_t vertexCount;
		size_t indexCount;

		size_t vertexOffsetInBuffer;
		size_t indexOffsetInBuffer;

	};



}