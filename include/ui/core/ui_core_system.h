#pragma once
#include "ui/systems/ui_vertex_buffer_management_system.h"
#include "ui/text/ui_font_manager.h"
#include "memory-management/gpu_buffer_manager.h"
#include "ecs.h"
#include "ui_shader_manager.h"
#include "low-level/gpu_texture_manager.h"

namespace TheEngine::UI
{
	class UICoreSystem
	{
	private:
	
		//owner
		UIVertexBufferManagementSystem m_uiVertexBufferManagementSystem;//requires GPUBufferManager


	    ECS::ECSEngine& m_ecsEngine;//not owner

		GPUTextureManager& m_gpuTextureManager;//not owner


	public:

		UICoreSystem(ECS::ECSEngine& ecsEngine, Memory::GPUBufferManager& gpuBufferManager, GPUTextureManager& gpuTextureManager);



		ECS::ECSEngine& getECSEngine();

		UIVertexBufferManagementSystem& getUIVertexBufferManagementSystem();

		UIFontManager& getUIFontManager();

		UIShaderManager& getUIShaderManager();

	};

}