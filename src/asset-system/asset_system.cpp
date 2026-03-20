#include "asset-system/asset_system.h"
#include "rendering-system/gpu-resource-system/gpu_resource_system.h"

namespace TheEngine::AssetSystem
{




	AssetSystem::AssetSystem(TheEngine::RenderingSystem::GPUResourceSystem& gpuResourceSystem) :
		m_animationResourceSystem(gpuResourceSystem.getGPUAnimationManager()),
		m_lightSystem(gpuResourceSystem.getGPULightManager()),
		m_materialSystem(gpuResourceSystem.getGPUMaterialManager()),
		m_meshSystem(gpuResourceSystem.getGPUMeshSystem())
		//m_textureSystem(gpuResourceSystem.)

	{

	}



	MeshSystem& AssetSystem::getMeshSystem()
	{
		return m_meshSystem;
	}



	MaterialSystem& AssetSystem::getMaterialSystem()
	{
		return m_materialSystem;
	}


/*
	TextureSystem& AssetSystem::getTextureSystem()
	{

		return m_textureSystem;
	}
	*/


	LightSystem& AssetSystem::getLightSystem()
	{

		return m_lightSystem;
	}



	AnimationResourceSystem& AssetSystem::getAnimationResourceSystem()
	{

		return m_animationResourceSystem;
	}




}