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




        const auto& it = m_nameToMaterialCreateInfoMap.find("Engine_Internal_Default_PBR_Material");




        if (it == m_nameToMaterialCreateInfoMap.end())

        {

            assert(false && "Material System : No default PBR material found");

        }



        const auto& defaultMaterial = it->second;


        gpuPBRMaterial.albedoTextureHandle = (createInfo.albedoTextureHandle.isValid())
            ? m_gpuMaterialManager.getBindlessTextureHandle(createInfo.albedoTextureHandle)
            : m_gpuMaterialManager.getBindlessTextureHandle(defaultMaterial.albedoTextureHandle);

        gpuPBRMaterial.metallicRoughnessTextureHandle = (createInfo.metallicRoughnessTextureHandle.isValid())
            ? m_gpuMaterialManager.getBindlessTextureHandle(createInfo.metallicRoughnessTextureHandle)
            : m_gpuMaterialManager.getBindlessTextureHandle(defaultMaterial.metallicRoughnessTextureHandle);

        gpuPBRMaterial.normalTextureHandle = (createInfo.normalTextureHandle.isValid())
            ? m_gpuMaterialManager.getBindlessTextureHandle(createInfo.normalTextureHandle)
            : m_gpuMaterialManager.getBindlessTextureHandle(defaultMaterial.normalTextureHandle);

        gpuPBRMaterial.occlusionTextureHandle = (createInfo.occlusionTextureHandle.isValid())
            ? m_gpuMaterialManager.getBindlessTextureHandle(createInfo.occlusionTextureHandle)
            : m_gpuMaterialManager.getBindlessTextureHandle(defaultMaterial.occlusionTextureHandle);

        gpuPBRMaterial.emissiveTextureHandle = (createInfo.emissiveTextureHandle.isValid())
            ? m_gpuMaterialManager.getBindlessTextureHandle(createInfo.emissiveTextureHandle)
            : m_gpuMaterialManager.getBindlessTextureHandle(defaultMaterial.emissiveTextureHandle);


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