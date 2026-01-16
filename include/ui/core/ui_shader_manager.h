#pragma once
#include "glad/glad.h"
#include <unordered_map>
#include "ui_core_data_structures.h"



namespace TheEngine::UI
{

	class UIShaderManager
	{

	private:

		std::unordered_map<UIVertexFormat, GLuint> m_vertexFormatToShaderProgram;


		GLuint compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode);


		GLuint m_textVAO;

	public:

		UIShaderManager();

	
		GLuint getShaderProgramForVertexFormat(UIVertexFormat uiVertexFormat);

		GLuint getTextVAO();

	};


}