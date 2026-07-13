#include <rendering-system/api-backend/opengl/opengl_pipeline_state_object.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{






	OpenglPipelineStateObject::OpenglPipelineStateObject(const RenderStateConfiguration& renderStateConfiguration) :

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



}
