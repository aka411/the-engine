#include <rendering-system/api-backend/opengl/opengl_shader_compiler.h>
#include <assert.h>
#include <iostream>



namespace TheEngine::RenderingSystem::OpenGLBackend
{

	GLuint compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode)
	{


		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertShaderSource = vertShaderCode.c_str();
		const char* fragShaderSource = fragShaderCode.c_str();

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

		// Check for fragment shader compilation errors
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


		return shaderProgram;

	}





}