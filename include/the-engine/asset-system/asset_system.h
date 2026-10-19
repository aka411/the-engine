#pragma once
#include "mesh_system.h"
#include "material_system.h"
#include "texture_system.h"
#include "light_system.h"
#include "animation_resource_system.h"



namespace TheEngine::RenderingSystem
{
	class GPUResourceSystem;
}
namespace TheEngine::Platform
{
	class Platform;
}


namespace TheEngine::AssetSystem
{

	class AssetSystem
	{

	private:



		//Owner of all
		AnimationResourceSystem m_animationResourceSystem;
		//LightSystem m_lightSystem;
		MaterialSystem m_materialSystem;
		MeshSystem m_meshSystem;
		TextureSystem m_textureSystem; 



	public:



		AssetSystem(TheEngine::Platform::Platform& platform, RenderingSystem::GPUResourceSystem& gpuResourceSystem);



		MeshSystem& getMeshSystem();
		MaterialSystem& getMaterialSystem();
		TextureSystem& getTextureSystem();

		//LightSystem& getLightSystem();
		AnimationResourceSystem& getAnimationResourceSystem();

	};






}