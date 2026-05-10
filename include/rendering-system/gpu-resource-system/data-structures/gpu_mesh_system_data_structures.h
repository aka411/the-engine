#pragma once
#include <assert.h>
#include <cstdint>
#include <memory-management/memory_block.h>


namespace TheEngine::RenderingSystem
{


	using MeshOffset = size_t;


	enum class BufferResourceUsageHint
	{
		STATIC,  // Upload once, use many times
		STREAM,   // Upload once, use once
		DYNAMIC // Upload often (once per frame/few frames)
	};


	//  Vertex Formats //

/*
								   VERTEX FORMAT

There is a implicit understanding what a single vertex Attribute is composed of

|---------------------Vertex Format(8 bits)----------------------------------------------------------------|
|-----------------------VertexAttribute Presence Bit--------------------------------||---DONT CARE BIT-----|
|--POSITION--||--NORMAL--||--UV_0--||---UV_1--||--TANGENT--||--WEIGHTS--||--JOINTS--||---DONT CARE BIT-----|

A uint8_t would suffice

|
*/


/*
|---------------------Vertex Format (8 bits)-------------------------------|
|-------------------VertexAttribute Presence Bit---------------------------|
| [7] DONT CARE                                                            |
| [6] JOINTS                                                               |
| [5] WEIGHTS                                                              |
| [4] TANGENT                                                              |
| [3] UV_1                                                                 |
| [2] UV_0                                                                 |
| [1] NORMAL                                                               |
| [0] POSITION                                                             |
|--------------------------------------------------------------------------|
*/

	enum class VertexAttributeBits : uint16_t
	{
		NONE = 0,
		POSITION = 1 << 0,
		NORMAL = 1 << 1,
		COLOR = 1 << 2,
		UV_0 = 1 << 3,
		UV_1 = 1 << 4,
		TANGENT = 1 << 5,
		WEIGHTS = 1 << 6,//A vec4 most times
		JOINTS = 1 << 7//  I have seen it be uint16 or uint32 i think
	};


	inline constexpr VertexAttributeBits operator|(VertexAttributeBits lhs, VertexAttributeBits rhs)
	{
		return static_cast<VertexAttributeBits>(
			static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs)
			);
	}


	inline constexpr VertexAttributeBits operator&(VertexAttributeBits lhs, VertexAttributeBits rhs)
	{
		return static_cast<VertexAttributeBits>(
			static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs)
			);
	}


	enum class VertexFormat : uint16_t
	{

		STANDARD = static_cast<uint16_t>(VertexAttributeBits::POSITION | VertexAttributeBits::NORMAL | VertexAttributeBits::UV_0),
		STANDARD_SKINNED = static_cast<uint16_t>(VertexAttributeBits::POSITION | VertexAttributeBits::NORMAL | VertexAttributeBits::UV_0 | VertexAttributeBits::WEIGHTS | VertexAttributeBits::JOINTS),
		STANDARD_2D = static_cast<uint16_t>(VertexAttributeBits::POSITION | VertexAttributeBits::UV_0),
		STANDARD_2_5D = static_cast<uint16_t>(VertexAttributeBits::POSITION | VertexAttributeBits::NORMAL | VertexAttributeBits::UV_0),
		//POSITION_ONLY

		//yeah color or colour
		STANDARD_2D_COLOUR = static_cast<uint16_t>(VertexAttributeBits::POSITION | VertexAttributeBits::COLOR)

	};


	enum class IndexFormat
	{
		UINT16,
		UINT32
	};











	struct MeshUploadData
	{
		VertexFormat vertexFormat = VertexFormat::STANDARD;
		BufferResourceUsageHint bufferResourceUsageHint = BufferResourceUsageHint::STATIC;

		TheEngine::Memory::MemoryBlock meshData;
		//size_t meshDataSizeInBytes = 0;
		//size_t vertexCount;


		bool hasIndex = false;

		IndexFormat indexFormat;
		TheEngine::Memory::MemoryBlock indexData;
		//size_t indexCount;
		//size_t indexDataSizeInBytes = 0;


	};




	struct DynamicMeshUploadInfo
	{

		VertexFormat vertexFormat;

		std::byte* verticesSrcPtr = nullptr;
		size_t verticesTotalBytes = 0;


		bool isIndexed = false;

		IndexFormat indexFormat;

		std::byte* indicesSrcPtr = nullptr;
		size_t indicesTotalBytes = 0;

	};







	//How about a mesh Id

	using MeshId = uint64_t;

	struct MeshInfo
	{
		MeshId meshId;
		VertexFormat vertexFormat;
		BufferResourceUsageHint bufferResourceUsageHint;

		bool hasIndex;
		IndexFormat indexFormat;

		size_t vertexDataSizeInBytes;
		size_t indexDataSizeInBytes;

		size_t vertexCount;
		size_t indexCount;

		size_t vertexOffsetInBuffer;
		size_t indexOffsetInBuffer;

	};



	/*
	Buffer A(Position) : vec3 positions for all meshes in the game.

	Buffer B(Attributes) : Normals, UVs, and Tangents for all meshes.

	Buffer C(Indices) : All uint32_t or uint16_t index data.


	struct DrawCall 
	{
	//For all vertex data , we will also have a verteformat 
	uint32_t vertexOffset; // Offset in the Position Stream
	uint32_t attributeOffset; // Offset in the UV/Normal Stream

	uint32_t indexOffset;  // Offset in the Global Index Buffer
	uint32_t indexCount;
	// ... 
};

*/




}