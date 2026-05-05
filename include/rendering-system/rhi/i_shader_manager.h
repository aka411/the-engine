#pragma once
#include <map>
#include <string>

#include <rendering-system/engine_handles.h>
#include <platform/path.h>




namespace TheEngine::RenderingSystem
{
	
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		COMPUTE,
		GEOMETRY,
		TESSELLATION_CONTROL,
		TESSELLATION_EVALUATION
	};
	
	struct ShaderCreateInfo
	{
		std::map< ShaderType, std::string> shaderSourceCodes; 
	};

	struct ShaderCreateInfoFromFile
	{
		std::map< ShaderType, TheEngine::Platform::Path> shaderSourceFilePaths; 
	};


	class IShaderManager
	{


	private:


	public:

		IShaderManager() = default;

		virtual ~IShaderManager() = default;

		//also add name string
		virtual const ShaderHandle createShader(ShaderCreateInfo shaderCreateInfo) = 0;




	};



}