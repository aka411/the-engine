#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h>
#include <rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h>


namespace TheEngine::RenderingSystem
{


	struct Camera
	{
		glm::mat4 projection;
		glm::mat4 view;
	};

	//Note : Two Mesh node can have same transform but different material etc 
	//how about buckets of draw call by vertex format , index format , material type and then id
	struct DrawCall
	{

		MaterialId materialId;//technically offset but other systems decide its meaning,cause i can make it so that its just messenger

		size_t vertexCount;
		size_t indexCount;

		size_t vertexDataSizeInBytes;
		size_t indexDataSizeInBytes;

		size_t vertexOffsetInBuffer;
		size_t indexOffsetInBuffer;


		glm::mat4 transformation;

	};



	struct DrawCallBucket
	{
		ShadingModel shadingModel;//material Type

		VertexFormat vertexFormat;
		IndexFormat indexFormat;
		BufferUsage bufferUsage;


		bool isIndexed;


		std::vector<DrawCall> drawCalls;//heap allocated bad , use custom alloctor and maintain pools of memory block for use

	};



}