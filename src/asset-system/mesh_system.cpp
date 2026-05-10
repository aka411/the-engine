#include "asset-system/mesh_system.h"
#include "rendering-system/gpu-resource-system/gpu_mesh_system.h"



namespace TheEngine::AssetSystem
{



    MeshSystem::MeshSystem(RenderingSystem::GPUMeshSystem & gpuMeshSystem) :

    m_gpuMeshSystem(gpuMeshSystem)

    {



    }



    void MeshSystem::storeMeshInfo(const std::string& name, const RenderingSystem::MeshInfo& meshInfo)
    {


        m_stringNameToMeshInfo.insert({ name ,meshInfo });


    }

    RenderingSystem::MeshInfo MeshSystem::getMeshInfo(const std::string& name) const
    {
        return m_stringNameToMeshInfo.at(name);
    }


    RenderingSystem::MeshInfo MeshSystem::uploadMeshData(RenderingSystem::MeshUploadData&& meshUploadData)
    {


       return m_gpuMeshSystem.uploadMeshData(std::move(meshUploadData));

    }


    RenderingSystem::MeshInfo MeshSystem::uploadDynamicMeshData(const RenderingSystem::DynamicMeshUploadInfo& dynamicMeshUploadInfo)
    {

        return m_gpuMeshSystem.uploadDynamicMeshData(dynamicMeshUploadInfo);


    }


 
    void MeshSystem::deleteMesh(RenderingSystem::MeshInfo meshInfo)
    {
        //need to remove from the buffer to free space etc

    }






}
