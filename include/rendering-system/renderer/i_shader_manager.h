#pragma once

namespace TheEngine::Platform
{
	class Platform;
	class Path;
}

namespace TheEngine::RenderingSystem
{


	class IShaderManager
	{


	private:

		//Need access to file system


	public:

		IShaderManager();

		virtual ~IShaderManager() = default;

		//TODO : Design needs more thought
		virtual const ShaderHandle createShader();


	};



}