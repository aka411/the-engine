#pragma once
#include "rendering-system/renderer/render-pass/i_render_pass.h"
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>




namespace TheEngine::RenderingSystem
{

	class ForwardRenderPass : public IRenderPass
	{


	private:

		PipelineHandle m_pipelineHandle;


		void setState();

	public:

		ForwardRenderPass(RenderingContext& renderingContext);


		virtual void excecute(const DrawCallBucket& drawCallbucket) override;



	};



}