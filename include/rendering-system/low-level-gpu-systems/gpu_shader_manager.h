#pragma once
#include <string>
#include <unordered_map>



namespace TheEngine::RenderingSystem
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

	public:


		GPUShaderManager();
		~GPUShaderManager();

		ShaderProgram getShader(const std::string& shaderName);
		ShaderProgram createAndStoreShader(const std::string& shaderName, ShaderCreateInfo shaderCreateInfo);

	};




}