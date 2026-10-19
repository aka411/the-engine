#pragma once
#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include <rendering-system/rhi/i_shader_manager.h>




namespace TheEngine::Platform
{
	class FileSystem;
}

namespace TheEngine::RenderingSystem::OpenGLBackend
{

	struct ShaderProgram
	{
		uint64_t shaderApiHandle;
	};


	class OpenglShaderManager : public IShaderManager
	{

	private:

		

		std::vector<GLuint> m_shaderPrograms;

	public:


		OpenglShaderManager(TheEngine::Platform::FileSystem& fileSystem);

		virtual ~OpenglShaderManager() override;



		virtual const ShaderHandle createShader(ShaderCreateInfo shaderCreateInfo) override;
		GLuint getNavtiveShaderHandle(const ShaderHandle& shaderHandle) const;
	};




}