#include <rendering-system/api-backend/opengl/opengl_pipeline_state_object.h>
#include <rendering-system/api-backend/opengl/opengl_pipeline_helper.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{
  
     GLenum toGLDataType(PrimitiveDataType type) {
        switch (type) {
        case PrimitiveDataType::FLOAT: return GL_FLOAT;
        default: return GL_FLOAT;
        }
    }




	OpenglPipelineStateObject::OpenglPipelineStateObject(PipelineDescriptor& pipelineDescriptor):
		IGPUPipeline(pipelineDescriptor)
	{


        glCreateVertexArrays(1, &m_vaoID);

       
        const auto& layout = pipelineDescriptor.layout;

        const auto& attributes = layout.getAttributes(); 

  //Creating VAO
        for (const auto& attr : attributes)
        {
            glEnableVertexArrayAttrib(m_vaoID, attr.location);

     

            glVertexArrayAttribFormat(
                m_vaoID,
                attr.location,
                attr.numOfComponents,
                GL_FLOAT, // Hardcoded for now 
                attr.normalize ? GL_TRUE : GL_FALSE,
                attr.offset
            );

          
            glVertexArrayAttribBinding(m_vaoID, attr.location, 0);
        }





	}




	void OpenglPipelineStateObject::bind()
	{

        const auto& desc = m_pipelineDescriptor;


        glUseProgram(desc.shader.shaderApiHandle);

        //  Rasterizer State
        if (desc.rasterizer.cullMode == CullMode::NONE)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(toGLCullMode(desc.rasterizer.cullMode));
        }

      //  glPolygonMode(GL_FRONT_AND_BACK, ToGLFillMode(desc.rasterizer.fillMode));
       // glLineWidth(desc.rasterizer.lineWidth);

        //  Depth & Stencil State
        if (desc.depthStencil.depthTestEnable)
        {
            //depth test has problem of needing to be cleared
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(toGLCompareOp(desc.depthStencil.depthCompareOp));
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        glDepthMask(desc.depthStencil.depthWriteEnable ? GL_TRUE : GL_FALSE);

        if (desc.depthStencil.stencilTestEnable)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }

        //  Blend State
        if (desc.blend.enabled)
        {
            glEnable(GL_BLEND);
            glBlendEquation(toGLBlendOp(desc.blend.colorBlendOp));
            glBlendFunc(
                toGLBlendFactor(desc.blend.srcColorFactor),
                toGLBlendFactor(desc.blend.dstColorFactor)
            );
        }
        else
        {
            glDisable(GL_BLEND);
        }

       glBindVertexArray(m_vaoID);
    }


	
}
