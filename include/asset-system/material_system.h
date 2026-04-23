#pragma once
#include <map>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/engine_handles.h>
#include <rendering-system/gpu-resource-system/data-structures/gpu_material_system_data_structures.h>


namespace TheEngine::RenderingSystem
{
	class GPUMaterialManager;
}



namespace TheEngine::AssetSystem
{

	/*     DESIGN
	
	- The output to game from this has to have direct significance to
	  renderingsystem like id should be an offset in buffer casue that way i can
	  easy make the draw call by just directly using the handle

	- Name, type, material 
	- No need for edit option
	- Need delete or store in cpu option though


	
	*/





	//This is component that game will see
	struct Material
	{
		//Store Name also??
		RenderingSystem::ShadingModel shadingModel;
		RenderingSystem::MaterialId materialId;
	};


	struct PBRMRCreateInfo
	{
		glm::vec4 baseColorFactor;
		glm::vec3 emissiveFactor;

		float metallicFactor;
		float roughnessFactor;



		RenderingSystem::TextureHandle albedoTextureHandle;
		RenderingSystem::TextureHandle metallicRoughnessTextureHandle;

		RenderingSystem::TextureHandle normalTextureHandle;
		RenderingSystem::TextureHandle occlusionTextureHandle;
		RenderingSystem::TextureHandle emissiveTextureHandle;
	};


	class MaterialSystem
	{

	private:


	
		RenderingSystem::GPUMaterialManager& m_gpuMaterialManager;

		//string to offset
		std::map<std::string, Material> m_materialNameToIdMap;
		std::map<std::string, PBRMRCreateInfo> m_nameToMaterialCreateInfoMap;


	public:

		void createDefaultInternalPBRMaterial(const PBRMRCreateInfo& createInfo);

		MaterialSystem(RenderingSystem::GPUMaterialManager& gpuMaterialManager);

		
		Material createPBRMaterial(const std::string& materialName, const PBRMRCreateInfo& createInfo);

		PBRMRCreateInfo getPBRMaterialCreateInfo(const std::string& materialName);

		Material getPBRMaterial(const std::string& materialName) const;


	};



}