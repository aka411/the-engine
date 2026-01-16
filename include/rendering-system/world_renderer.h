#pragma once
#include "low-level/rendering_system_data_types.h"
#include "low-level/world_vertex_buffer_management_system.h"
#include "low-level/shader_manager.h"
#include "low-level/vertex_format_manager.h"
#include "components.h"
#include "camera.h"
#include "animation/animation_system.h"

#include "world_renderer_data_structures.h"
#include "render_command_buffer_manager.h"
#include "object_data_buffer_manager.h"
#include "light-system/light_system.h"

#include <memory>

namespace TheEngine
{

	class WorldRenderer
	{

	private:

		WorldVertexBufferManagementSystem& m_worldVertexBufferManagementSystem;
		VertexFormatManager& m_vertexFormatManager;
		ShaderManager m_shaderManager;

		GPUMaterialSystem& m_gpuMaterialSystem;

		Memory::GPUBufferManager m_gpuBufferManager;

		Memory::GPUBufferInfo m_cameraBufferInfo;


		Animation::AnimationSystem& m_animationSystem;


		RenderCommandBufferManager& m_renderCommandBufferManager;
		ObjectDataBufferManager& m_objectDataBufferManager;


		/*Lighting*/

		LightSystem& m_lightSystem;


		/*Timing Info*/
		//To keep header clean from opengl 
		std::unique_ptr<GLuint> m_timeQuery;
		float m_gpuTimeMS = 0;
	public:

		WorldRenderer(
			VertexFormatManager& vertexFormatManager,
			WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem,
			GPUMaterialSystem& gpuMaterialSystem, Animation::AnimationSystem& animationSystem,
			RenderCommandBufferManager& m_renderCommandBufferManager,
		    ObjectDataBufferManager& m_objectDataBufferManager,
			LightSystem& lightSystem
		);

		void setViewportDimension(int width, int height);	

		void startFrame(TheEngine::Camera& camera);

		//void render(std::unordered_map<VertexFormat, std::vector<RenderCommand>>& vertexFormatToRenderCommands);
		
		//void depthPrePass();//setting state is a headache might need to rethink this class


		void indirectDrawArray(const VertexFormat vertexFormat, const size_t byteOffset, const size_t count);
		void indirectDrawIndexed(const VertexFormat vertexFormat, const IndexType indexType, const size_t byteOffset, const size_t count);
		

		void endFrame();

		float getGPUTimeForLastFrameMS();


	};

}