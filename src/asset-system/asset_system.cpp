#include "asset-system/asset_system.h"
#include "rendering-system/gpu-resource-system/gpu_resource_system.h"

namespace TheEngine::AssetSystem
{




	AssetSystem::AssetSystem(TheEngine::Platform::Platform& platform, RenderingSystem::GPUResourceSystem& gpuResourceSystem) :

		m_animationResourceSystem(gpuResourceSystem.getGPUAnimationManager()),
		//m_lightSystem(gpuResourceSystem.getGPULightManager()),
		m_materialSystem(gpuResourceSystem.getGPUMaterialManager()),
		m_meshSystem(gpuResourceSystem.getGPUMeshSystem()),

		m_textureSystem(gpuResourceSystem.getTextureManager(), platform)

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
	LightSystem& AssetSystem::getLightSystem()
	{

		return m_lightSystem;
	}*/



	AnimationResourceSystem& AssetSystem::getAnimationResourceSystem()
	{

		return m_animationResourceSystem;
	}



	TextureSystem& AssetSystem::getTextureSystem()
	{

		return m_textureSystem;
	}

}