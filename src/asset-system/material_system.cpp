#include "asset-system/material_system.h"
#include "rendering-system/gpu-resource-system/gpu_material_manager.h"
#include "rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h"

namespace TheEngine::AssetSystem
{



    MaterialSystem::MaterialSystem(RenderingSystem::GPUMaterialManager& gpuMaterialManager) :

        m_gpuMaterialManager(gpuMaterialManager)

    {



    }


    Material  MaterialSystem::createPBRMaterial(const std::string& materialName, const PBRMRCreateInfo& createInfo)
    {
       
        TheEngine::RenderingSystem::GPUPBRMaterial gpuPBRMaterial;

        // Textures
        gpuPBRMaterial.albedoTextureHandle = createInfo.albedoTextureInfo.resisdentHandle;
        gpuPBRMaterial.emissiveTextureHandle = createInfo.emissiveTextureInfo.resisdentHandle;
        gpuPBRMaterial.normalTextureHandle = createInfo.normalTextureInfo.resisdentHandle;
        gpuPBRMaterial.metallicRoughnessTextureHandle = createInfo.metallicRoughnessTextureInfo.resisdentHandle;
        gpuPBRMaterial.occlusionTextureHandle = createInfo.occlusionTextureInfo.resisdentHandle;

        // Factors 
        gpuPBRMaterial.baseColorFactor = createInfo.baseColorFactor;
        gpuPBRMaterial.roughnessFactor = createInfo.roughnessFactor;
        gpuPBRMaterial.emissiveFactor = createInfo.emissiveFactor;
        gpuPBRMaterial.metallicFactor = createInfo.metallicFactor;
     
        m_nameToMaterialCreateInfoMap[materialName] = createInfo;
   
        RenderingSystem::MaterialId materialId = m_gpuMaterialManager.
            uploadStruct<TheEngine::RenderingSystem::GPUPBRMaterial>
            (TheEngine::RenderingSystem::ShadingModel::PBR_METALLIC_ROUGHNESS, gpuPBRMaterial);



        //need better mappping
        Material material{ TheEngine::RenderingSystem::ShadingModel::PBR_METALLIC_ROUGHNESS,materialId };

        m_materialNameToIdMap[materialName] = material;


        return material;
    }


    PBRMRCreateInfo MaterialSystem::getPBRMaterialCreateInfo(const std::string& materialName)
    {
        auto it = m_nameToMaterialCreateInfoMap.find(materialName);

        if (it == m_nameToMaterialCreateInfoMap.end())
        {
            
            // to catch cases where a non-existent material is requested.
            assert(false && ("Failed To Find Material Create Info: " + materialName).c_str());

            
            return PBRMRCreateInfo{};
        }

        return it->second;
    }



    Material MaterialSystem::getPBRMaterial(const std::string& materialName) const
    {

        auto it =  m_materialNameToIdMap.find(materialName);

        if (it == m_materialNameToIdMap.end())
        {

            assert(false && ("Failed To Find Material : " + materialName).c_str());


        }


        return it->second;
    }




}