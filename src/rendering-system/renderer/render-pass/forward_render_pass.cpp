#include <string>
#include <glm/gtc/type_ptr.hpp>

#include <rendering-system/renderer/rendering_context.h>
#include <rendering-system/renderer/render-pass/forward_render_pass.h>
#include <rendering-system/low-level-gpu-systems/low_level_gpu_system.h>
#include <rendering-system/low-level-gpu-systems/gpu_shader_manager.h>

#include <rendering-system/gpu-resource-system/gpu_resource_system.h>
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>
#include <rendering-system/gpu-resource-system/gpu_material_manager.h>

#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>
#include <rendering-system/low-level-gpu-systems/gpu_pipeline_manager.h>

#include <platform/path.h>//BAD!!!


namespace TheEngine::RenderingSystem
{



    void ForwardRenderPass::setState()
    {
       
        //get pipeline handle and bind
        m_lowLevelGPUSystem.getGPUPipelineManager().getGPUPipelineState(m_pipelineHandle).bind();

        GPUBufferInfo gpuBufferInfo = m_gpuResourceSystem.getGPUMeshSystem().getGPUBufferInfoForVetexFormat(VertexFormat::STANDARD, BufferUsage::STATIC);
        glBindVertexBuffer(0, gpuBufferInfo.bufferHandle, 0, 32);

      //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_animationSystem.getSkeletalAnimationManager().getJointMatrixSSBO().bufferHandle);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_gpuResourceSystem.getGPUMaterialManager().getGPUBufferInfoForMaterial(ShadingModel::PBR_METALLIC_ROUGHNESS).bufferHandle);
       //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_lightSystem.getLightSSBO().bufferHandle);


       


       
        //glViewport(0, 0, m_renderingContext.swapChainImageWidth, m_renderingContext.swapChainImageHeight);
        glViewport(0, 0, 400, 400);
        glUniformMatrix4fv(0, 1, GL_FALSE, &m_renderingContext.projection[0][0]);
        glUniformMatrix4fv(1, 1, GL_FALSE, &m_renderingContext.view[0][0]);

        glClearDepth(1.0);
        glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    }


	ForwardRenderPass::ForwardRenderPass(RenderingContext& renderingContext) :
		IRenderPass(renderingContext)
	{

        VertexLayout pbrLayout;//our vao

        pbrLayout.addAttribute(0, 3, PrimitiveDataType::FLOAT, false, 0);  // Position
        pbrLayout.addAttribute(1, 3, PrimitiveDataType::FLOAT, false, 12); // Normal
        pbrLayout.addAttribute(2, 2, PrimitiveDataType::FLOAT, false, 24); // UV

       






       DepthStencilState depthStencilState;
       RasterizerState rasterizerState;
       BlendState blendState;

       
       



     
        PipelineDescriptor desc;
        desc.shader = m_lowLevelGPUSystem.
            getGPUShaderManager().
            loadCreateAndStoreShader(
                "ForwardRenderPass_Shader",
                TheEngine::Platform::Path(TheEngine::Platform::MountPoint::ENGINE, "shaders/forward_vert.glsl"),
                TheEngine::Platform::Path(TheEngine::Platform::MountPoint::ENGINE, "shaders/forward_frag.glsl"));
        desc.layout = pbrLayout;
        desc.depthStencil = depthStencilState;
        desc.rasterizer = rasterizerState;
        desc.blend = blendState;



        
        m_pipelineHandle = m_lowLevelGPUSystem.getGPUPipelineManager().createPipeline(desc);





	}






	void ForwardRenderPass::excecute(const DrawCallBucket& drawCallbucket)
	{

        setState();
        
        for (auto& drawCall : drawCallbucket.drawCalls)
        {


           const glm::mat4& modelMatrix = drawCall.transformation;
           const uint32_t& materialId = drawCall.materialId; 

            glUniformMatrix4fv(10, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform1ui(11, materialId);


            GLint firstVertex = static_cast<GLint>(drawCall.vertexOffsetInBuffer / 32);

            glDrawArrays(
                GL_TRIANGLES,
                firstVertex,
                static_cast<GLsizei>(drawCall.vertexCount)
            );
       
        }





	}



}