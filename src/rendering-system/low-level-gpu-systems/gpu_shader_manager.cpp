#include <rendering-system/low-level-gpu-systems/gpu_shader_manager.h>
#include <assert.h>
#include <glad/glad.h>
#include <rendering-system/api-backend/opengl/opengl_shader_compiler.h>


namespace TheEngine::RenderingSystem
{



	GPUShaderManager::GPUShaderManager()
	{


	}


	GPUShaderManager::~GPUShaderManager()
	{


	}

	ShaderProgram GPUShaderManager::getShader(const std::string& shaderName)
	{

		auto it = m_shaderPrograms.find(shaderName);
		if (it != m_shaderPrograms.end()) {
			return  it->second;
		}

		assert(false && ("Failed To Get ShaderProgram : " + shaderName).c_str());


		return ShaderProgram{};
	}



	ShaderProgram GPUShaderManager::createAndStoreShader(const std::string& shaderName, ShaderCreateInfo shaderCreateInfo)
	{
		


		GLuint shaderProgramOpenglHandle = TheEngine::RenderingSystem::OpenGLBackend::compileShader(shaderCreateInfo.vertexShaderString, shaderCreateInfo.fragmentShaderString);

		m_shaderPrograms[shaderName] = ShaderProgram{ shaderProgramOpenglHandle };
		//casting
		return ShaderProgram{ shaderProgramOpenglHandle };
	}



}