#pragma once
#include "low-level/gpu_texture_manager.h"


namespace TheEngine
{
	class ShadowSystem
	{
	private:

		GPUTextureManager& m_gpuTextureManager;

	public:

		ShadowSystem(GPUTextureManager& gpuTextureManager);

	};

}