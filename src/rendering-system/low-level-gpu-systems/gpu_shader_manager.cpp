#include <rendering-system/low-level-gpu-systems/gpu_shader_manager.h>
#include <assert.h>
#include <glad/glad.h>
#include <rendering-system/api-backend/opengl/opengl_shader_compiler.h>
#include <platform/platform.h>

namespace TheEngine::RenderingSystem
{



	GPUShaderManager::GPUShaderManager(TheEngine::Platform::Platform& platform):
		m_platform(platform)
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







	ShaderProgram GPUShaderManager::loadCreateAndStoreShader(const std::string& shaderName, const TheEngine::Platform::Path& vertexShaderPath, const TheEngine::Platform::Path& fragmentShaderPath)
	{

		TheEngine::Platform::File vertexShaderfile =m_platform.getFileSystem().open(vertexShaderPath);
		TheEngine::Platform::File fragmentShaderfile = m_platform.getFileSystem().open(fragmentShaderPath);

		ShaderCreateInfo shaderCreateInfo;
		shaderCreateInfo.vertexShaderString = std::string(reinterpret_cast<char*>(vertexShaderfile.data()), vertexShaderfile.size()) ;
		shaderCreateInfo.fragmentShaderString = std::string(reinterpret_cast<char*>(fragmentShaderfile.data()), fragmentShaderfile.size());



		return createAndStoreShader(shaderName,shaderCreateInfo);


	}


}