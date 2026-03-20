#include "asset-system/mesh_system.h"
#include "rendering-system/gpu-resource-system/gpu_mesh_system.h"



namespace TheEngine::AssetSystem
{



    MeshSystem::MeshSystem(RenderingSystem::GPUMeshSystem & gpuMeshSystem) :

    m_gpuMeshSystem(gpuMeshSystem)

    {



    }

    RenderingSystem::MeshInfo MeshSystem::uploadMeshData(RenderingSystem::MeshUploadData&& meshUploadData)
    {


       return m_gpuMeshSystem.uploadMeshData(std::move(meshUploadData));

    }

 







}
