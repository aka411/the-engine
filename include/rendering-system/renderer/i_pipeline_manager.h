#pragma once


namespace TheEngine::Platform
{
	class Platform;
	class Path;
}


namespace TheEngine::RenderingSystem
{
	
	//What should this class do
	class IPipelineManager
	{

	private:

		//Might need access to filesystem -> for loading pso json from disk


		//We only store derived objects
	public:
		
		IPipelineManager();
		virtual ~IPipelineManager() = default;

		//TODO : SOLVE SLICING ISSUE, what if we used stable vector?
		//Weak_ptr??
		//Handle Based?
		virtual const PipelineHandle createPipelineStateObject(const RenderStateConfiguration& renderStateConfiguration) = 0;
		
		//Reference returning is tricky , if stored in vector might cause dangling reference 
		virtual const IPipelineStateObject& getPipelineStateObject(const PipelineHandle& pipelineHandle) = 0;


		//virtual PipelineHandle loadPipelineConfigFromFile(const TheEngine::Platform::Path& configPath) = 0;
		virtual void destroyPipelineStateObject(const PipelineHandle& pipelineHandle) = 0

	};



}