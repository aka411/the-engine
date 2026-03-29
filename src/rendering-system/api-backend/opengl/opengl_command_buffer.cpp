

namespace TheEngine::RenderingSystem::OpenGLBackend
{



    OpenglCommandBuffer::OpenglCommandBuffer(const GraphicsAPI& graphicsAPI) ://we need to send in a pso cache
        ICommandBuffer(graphicsAPI)
    {
    
    
    
    }


    ~OpenglCommandBuffer() = default;

    virtual voidOpenglCommandBuffer::begin() override
    {
        //do nothing
    }

   
    virtual void voidOpenglCommandBuffer::end() override
    {
        //do nothing
    }


    virtual void voidOpenglCommandBuffer::execute() override
    {
        //do nothing
    }

    // Pipeline & State
    //Maybe not handle
    //Maybe IPipelineStateObject -> Render Configuration , for opengl will also contain VAO
    virtual void OpenglCommandBuffer::bindPipeline(const IPipelineStateObject& pipelineStateObject) override
    {

        //TODO : Need to redesign, PSO is just an object that will contain a render cnfig and also vao

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



virtual void setViewPort(float x, float y, float width, float height) override
{
    glViewport(x, y, width, height);
}




// Resource Binding
  //Repalce with IDescriptorSet? ---> ResourceGroupLayout
virtual void bindDescriptorSet(uint32_t setIndex, DescriptorSetHandle set) = 0;// even with new design above we need this
//virtual void bindResourceGroupSet(uint32_t setIndex, DescriptorSetHandle set) = 0;

 //Replaced by above one method
   //virtual void bindSSBO(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;
   //virtual void bindUBO(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;

virtual void bindVertexBuffer(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;
virtual void bindIndexBuffer(GPUBufferInfo gpuBufferInfo) = 0;


virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) = 0;



//Draw Commands
virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) = 0;
virtual void draw(uint32_t vertexCount, uint32_t firstVertex) = 0;





}