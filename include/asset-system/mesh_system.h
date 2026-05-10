#pragma once
#include <rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h>
#include <unordered_map>
#include <string>



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

		std::unordered_map<std::string, RenderingSystem::MeshInfo> m_stringNameToMeshInfo;
	public:

		MeshSystem(RenderingSystem::GPUMeshSystem& gpuMeshSystem);



		void storeMeshInfo(const std::string& name, const RenderingSystem::MeshInfo& meshInfo);
		RenderingSystem::MeshInfo getMeshInfo(const std::string& name) const;

		//Need method to delete mesh 
		RenderingSystem::MeshInfo uploadMeshData(RenderingSystem::MeshUploadData&& meshUploadData);
		RenderingSystem::MeshInfo uploadDynamicMeshData(const RenderingSystem::DynamicMeshUploadInfo& dynamicMeshUploadInfo);



		//How will we handle dynamic allocations that dont need freeing
		//Design of freeing of resources is still under developement 
		void deleteMesh(RenderingSystem::MeshInfo meshInfo);

	};


}