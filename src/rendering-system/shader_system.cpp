#include <rendering-system/shader_system.h>
#include <rendering-system/rhi/i_shader_manager.h>
#include <platform/file_system.h>


namespace TheEngine::RenderingSystem
{



		ShaderSystem::ShaderSystem(TheEngine::Platform::FileSystem& fileSystem, IShaderManager& shaderManager) :
			m_fileSystem(fileSystem),
			m_shaderManager(shaderManager)
		{

		}







		const ShaderHandle ShaderSystem::createShaderFromFile(ShaderCreateInfoFromFile createInfoFromFile)
		{
			return m_shaderManager.createShader(loadShaderCreateInfoFromFile(createInfoFromFile));

		}


		ShaderCreateInfo ShaderSystem::loadShaderCreateInfoFromFile(const ShaderCreateInfoFromFile& createInfoFromFile) const
		{
			ShaderCreateInfo createInfo;
			for (const auto& [shaderType, filePath] : createInfoFromFile.shaderSourceFilePaths)
			{
				auto file = m_fileSystem.open(filePath);

				std::string shaderCode(reinterpret_cast<char*>(file.data()), file.size());

				createInfo.shaderSourceCodes[shaderType] = shaderCode;
			}
			return createInfo;
		}







}