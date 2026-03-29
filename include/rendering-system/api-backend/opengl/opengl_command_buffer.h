#pragma once


namespace TheEngine::RenderingSystem::OpenGLBackend
{





	class OpenglCommandBuffer : public ICommandBuffer
	{


	private:

		//store current pipeline state for tracking changes and avoiding setting state that already is set
		//if we do this --->>> NEVER EVER MAKE RAW OPENGL CALLS TO CHANGE STATE

	public:

		OpenglCommandBuffer(GraphicsAPI graphicsAPI);


		virtual ~OpenglCommandBuffer() = default;

		// Recording Control
		virtual void begin() override;


		virtual void end() override;



		virtual void execute() override;


		// Pipeline & State
		virtual void bindPipeline(const IPipelineStateObject& pipelineStateObject) override;




		virtual void setViewPort(float x, float y, float width, float height) override;




		// Resource Binding
		  //Repalce with IDescriptorSet? ---> ResourceGroupLayout
		virtual void bindDescriptorSet(uint32_t setIndex, DescriptorSetHandle set) override;// even with new design above we need this
		//virtual void bindResourceGroupSet(uint32_t setIndex, DescriptorSetHandle set) = 0;

		 //Replaced by above one method
		   //virtual void bindSSBO(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;
		   //virtual void bindUBO(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;

		virtual void bindVertexBuffer(int bindingPoint, GPUBufferInfo gpuBufferInfo) override;
		virtual void bindIndexBuffer(GPUBufferInfo gpuBufferInfo) = 0;


		virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) override;



		//Draw Commands
		virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) override;
		virtual void draw(uint32_t vertexCount, uint32_t firstVertex) override;



	}

}