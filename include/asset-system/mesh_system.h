#pragma once
#include "rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h"

namespace  TheEngine::RenderingSystem
{
	class GPUMeshSystem;

}

namespace TheEngine::AssetSystem
{



	class MeshSystem
	{

	private:



		RenderingSystem::GPUMeshSystem& m_gpuMeshSystem;


	public:

		MeshSystem(RenderingSystem::GPUMeshSystem& gpuMeshSystem);

		//Need method to delete mesh 
		RenderingSystem::MeshInfo uploadMeshData(RenderingSystem::MeshUploadData&& meshUploadData);

		//How will we handle dynamic allocations that dont need freeing
		//Design of freeing of resources is still under developement 
		void deleteMesh(RenderingSystem::MeshInfo meshInfo);

	};


}