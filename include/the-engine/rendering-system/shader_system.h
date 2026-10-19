#pragma once
#include <rendering-system/rhi/i_shader_manager.h>

namespace TheEngine::Platform
{
	class FileSystem;

}


namespace TheEngine::RenderingSystem
{
	class IShaderManager;

	class ShaderSystem
	{
	private:


		TheEngine::Platform::FileSystem& m_fileSystem;

		IShaderManager& m_shaderManager;

		ShaderCreateInfo loadShaderCreateInfoFromFile(const ShaderCreateInfoFromFile& createInfoFromFile) const;


	public:

		ShaderSystem(TheEngine::Platform::FileSystem& fileSystem, IShaderManager& shaderManager);

		~ShaderSystem() = default;

		const ShaderHandle createShaderFromFile(ShaderCreateInfoFromFile shaderCreateInfoFromFile);

	








	};



}