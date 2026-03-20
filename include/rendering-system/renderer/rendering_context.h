#pragma once
#include <unordered_map>
#include <string>
#include "rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h"
#include "glm/mat4x4.hpp"



namespace TheEngine::RenderingSystem
{

	class LowLevelGPUSystem;
	class GPUResourceSystem;



	class RenderingContext
	{

	private:

		LowLevelGPUSystem& m_lowLevelGPUSystem;

		GPUResourceSystem& m_gpuResourceSystem;

		//To store intermeadiate resources
		std::unordered_map<std::string, TextureInfo> m_textureResources;
		//std::unordered_map<std::string, bufferInfo> m_bufferResources;


		//should i add camera in here
	public:

		glm::mat4 projection;
		glm::mat4 view;

		int swapChainImageWidth = 0;
		int swapChainImageHeight = 0;

		RenderingContext(LowLevelGPUSystem& lowLevelGPUSystem, GPUResourceSystem& gpuResourceSystem);
		
		


		//Replace with TextureCreateInfo
		void addTextureResource(const std::string& name, TextureInfo textureInfo);
		TextureInfo getTextureResource(const std::string& name);



		//For getting Systems

		GPUResourceSystem& getGPUResourceSystem();
		LowLevelGPUSystem& getLowLevelGPUSystem();



	};


}