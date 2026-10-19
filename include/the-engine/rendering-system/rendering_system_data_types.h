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


	struct DrawCall
	{

		MaterialId materialId;//technically offset 

		size_t vertexCount;
		size_t indexCount;

		size_t vertexOffsetInBuffer;
		size_t indexOffsetInBuffer;


		glm::mat4 transformation;

	};



	struct DrawCallBucket
	{
		ShadingModel shadingModel;//material Type

		VertexFormat vertexFormat;
		IndexFormat indexFormat;
		BufferResourceUsageHint bufferResourceUsageHint;


		bool isIndexed;


		std::vector<DrawCall> drawCalls;

	};





	//User Data passed to each pass

	struct UserPassData
	{
		void* arg{ nullptr };

	};


}