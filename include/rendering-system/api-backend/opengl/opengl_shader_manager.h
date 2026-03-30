#pragma once
#include <string>
#include <unordered_map>



namespace TheEngine::RenderingSystem::OpenGLBackend
{

	struct ShaderProgram
	{
		uint64_t shaderApiHandle;//cast to specific type
	};

	struct ShaderCreateInfo
	{
		std::string vertexShaderString;
		std::string fragmentShaderString;
	};

	class GPUShaderManager
	{

	private:

		std::unordered_map<std::string, ShaderProgram> m_shaderPrograms;
		TheEngine::Platform::Platform& m_platform;

	public:


		GPUShaderManager(TheEngine::Platform::Platform& platform);//ToDo : Remove this dependency
		~GPUShaderManager();

		ShaderProgram getShader(const std::string& shaderName);
		ShaderProgram createAndStoreShader(const std::string& shaderName, ShaderCreateInfo shaderCreateInfo);

		//This method is temporary and will later need to be removed to remove dependency on TheEngine::Platform::Platform
		ShaderProgram loadCreateAndStoreShader(const std::string& shaderName, const TheEngine::Platform::Path& vertexShaderPath, const TheEngine::Platform::Path& fragmentShaderPath);

	};




}