#include "../../../include/backends/opengl/opengl_pipeline_state.h"


namespace TheEngine::Graphics
{



	void OpenGLPipelineState::Bind() const
	{
		glUseProgram(m_shaderProgram);
		glBindVertexArray(m_vao);

		glPolygonMode(GL_FRONT_AND_BACK, m_rasterizationState.polygonMode);

		glFrontFace(m_rasterizationState.frontFace);

		if (m_rasterizationState.cullMode != GL_NONE)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(m_rasterizationState.cullMode);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		if (m_depthStencilState.depthTestEnable)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(m_depthStencilState.depthCompareOp);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (m_depthStencilState.depthWriteEnable)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		if (m_depthStencilState.stencilTestEnable)
		{
			glEnable(GL_STENCIL_TEST);
			// Additional stencil state setup would go here
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}

	}




	OpenGLPipelineState::OpenGLPipelineState(const OpenGLInputAssemblyState& inputAssemblyState, const OpenGLRasterizationState& rasterizationState, const OpenGLDepthStencilState& depthStencilState, GLuint vao, GLuint shaderProgram)
		: m_inputAssemblyState(inputAssemblyState),
		m_rasterizationState(rasterizationState),
		m_depthStencilState(depthStencilState),
		m_vao(vao),
		m_shaderProgram(shaderProgram)
	{

	}
	TheEngine::Graphics::OpenGLPipelineState::~OpenGLPipelineState()
	{
	}
}