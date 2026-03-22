#include "asset-system/material_system.h"
#include "rendering-system/gpu-resource-system/gpu_material_manager.h"
#include "rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h"

namespace TheEngine::AssetSystem
{



    void MaterialSystem::createDefaultInternalPBRMaterial(const PBRMRCreateInfo& createInfo)
    {
        m_nameToMaterialCreateInfoMap["Engine_Internal_Default_PBR_Material"] = createInfo;
    }





    MaterialSystem::MaterialSystem(RenderingSystem::GPUMaterialManager& gpuMaterialManager) :

        m_gpuMaterialManager(gpuMaterialManager)

    {



    }


    Material  MaterialSystem::createPBRMaterial(const std::string& materialName, const PBRMRCreateInfo& createInfo)
    {
       
        TheEngine::RenderingSystem::GPUPBRMaterial gpuPBRMaterial;


        /*
        // Textures
        gpuPBRMaterial.albedoTextureHandle = createInfo.albedoTextureInfo.resisdentHandle;
        gpuPBRMaterial.emissiveTextureHandle = createInfo.emissiveTextureInfo.resisdentHandle;
        gpuPBRMaterial.normalTextureHandle = createInfo.normalTextureInfo.resisdentHandle;
        gpuPBRMaterial.metallicRoughnessTextureHandle = createInfo.metallicRoughnessTextureInfo.resisdentHandle;
        gpuPBRMaterial.occlusionTextureHandle = createInfo.occlusionTextureInfo.resisdentHandle;
        */


        const auto& it = m_nameToMaterialCreateInfoMap.find("Engine_Internal_Default_PBR_Material");




        if (it == m_nameToMaterialCreateInfoMap.end())

        {

            assert(false && "Material System : No default PBR material found");

        }



        const auto& defaultMaterial = it->second;



        gpuPBRMaterial.albedoTextureHandle = (createInfo.albedoTextureInfo.residentHandle != 0)
            ? createInfo.albedoTextureInfo.residentHandle
            : defaultMaterial.albedoTextureInfo.residentHandle;

        gpuPBRMaterial.metallicRoughnessTextureHandle = (createInfo.metallicRoughnessTextureInfo.residentHandle != 0)
            ? createInfo.metallicRoughnessTextureInfo.residentHandle
            : defaultMaterial.metallicRoughnessTextureInfo.residentHandle;

        gpuPBRMaterial.normalTextureHandle = (createInfo.normalTextureInfo.residentHandle != 0)
            ? createInfo.normalTextureInfo.residentHandle
            : defaultMaterial.normalTextureInfo.residentHandle;

        gpuPBRMaterial.occlusionTextureHandle = (createInfo.occlusionTextureInfo.residentHandle != 0)
            ? createInfo.occlusionTextureInfo.residentHandle
            : defaultMaterial.occlusionTextureInfo.residentHandle;

        gpuPBRMaterial.emissiveTextureHandle = (createInfo.emissiveTextureInfo.residentHandle != 0)
            ? createInfo.emissiveTextureInfo.residentHandle
            : defaultMaterial.emissiveTextureInfo.residentHandle;


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