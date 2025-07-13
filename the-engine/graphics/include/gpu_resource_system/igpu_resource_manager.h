

#pragma once
namespace TheEngine::Graphics
{

	class IGPUResourceManager
	{
	public:

		virtual IGPUResource* getResource(const TheEngine::Core::ResourceHandle handle) const = 0;
	};


}