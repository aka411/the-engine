#pragma once

namespace TheEngine::Graphics
{



	struct OpenGLInputAssemblyState
	{
		GLenum topology;

	};

	struct OpenGLRasterizationState
	{
		GLenum polygonMode = GL_FILL;
		GLenum cullMode = GL_BACK;
		GLenum frontFace = GL_CCW; // Counter-clockwise


	};





	struct OpenGLDepthStencilState
	{
		GLboolean depthTestEnable = GL_TRUE;
		GLboolean depthWriteEnable = GL_TRUE;
		GLenum depthCompareOp = GL_LESS;

		GLboolean stencilTestEnable = GL_FALSE;



	};




	class OpenGLPipelineState : public IGPUPipelineState
	{
	private:
		const GLuint m_vao;
		const GLuint m_shaderProgram;

		const OpenGLInputAssemblyState m_inputAssemblyState;
		const OpenGLRasterizationState m_rasterizationState;
		const OpenGLDepthStencilState m_depthStencilState;


		// Render pass compatibility is typically handled by the current OpenGL context and framebuffer,
		// so we won't store it here explicitly like in Vulkan.
		// OpenGL doesn't have a direct equivalent to Vulkan's render pass compatibility,
		// but we can manage it through the current framebuffer and shader program.
		//so it would be a waste of space to store it here.



	public:
		OpenGLPipelineState(const OpenGLInputAssemblyState& inputAssemblyState,
		const OpenGLRasterizationState& rasterizationState,
		const OpenGLDepthStencilState& depthStencilState, GLuint vao, GLuint shaderProgram);

		~OpenGLPipelineState() ;


		void Bind() const ;
		// OpenGL-specific methods for binding and using the pipeline state



	};
}
