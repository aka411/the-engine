#include <rendering-system/api-backend/opengl/opengl_shader_manager.h>

#include <assert.h>
#include <iostream>


namespace TheEngine::RenderingSystem::OpenGLBackend
{




	OpenglShaderManager::OpenglShaderManager(TheEngine::Platform::FileSystem& fileSystem) :
		IShaderManager(fileSystem)
	{


	}



	OpenglShaderManager::~OpenglShaderManager()
	{

	}



	const ShaderHandle OpenglShaderManager::createShader(ShaderCreateInfo shaderCreateInfo)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertShaderSource = shaderCreateInfo.shaderSourceCodes[ShaderType::VERTEX].c_str();
		const char* fragShaderSource = shaderCreateInfo.shaderSourceCodes[ShaderType::FRAGMENT].c_str();

		glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
		glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);



		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			assert(false && "Vertex Shader Compilation Failed");
		}


		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			assert(false && "Fragment Shader Compilation Failed");
		}



		GLuint shaderProgram;

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			assert(false && "Shader Program Linking Failed");
		}


		//cleanup
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		ShaderHandle shaderHandle;
		shaderHandle.id = m_shaderPrograms.size() + 1; //Handles are 1-based indexed, 0 is invalid
		m_shaderPrograms.push_back(shaderProgram);

		return shaderHandle;
	}

	GLuint OpenglShaderManager::getNavtiveShaderHandle(const ShaderHandle& shaderHandle) const
	{
		assert(shaderHandle.id > 0 && "OpenglShaderManager : Invalid ShaderHandle");
		assert(shaderHandle.id <= m_shaderPrograms.size() && "OpenglShaderManager : Tried to access invalid shader");

		return m_shaderPrograms[shaderHandle.id - 1];

	}

}