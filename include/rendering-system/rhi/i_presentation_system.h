#pragma once


namespace TheEngine::RenderingSystem
{
	class ICommandBuffer;


	class IPresentationSystem
	{
	
	
	private:



	public:

		IPresentationSystem() = default;

		virtual ~IPresentationSystem() = default;
	
		virtual void startFrame() = 0 ;

		virtual const int getCurrentFrameIndex() const = 0;

		virtual ICommandBuffer& getRenderCommandBuffer() = 0;


		virtual void setWindowSize(const WindowExtent& windowExtent) = 0;

			//Implcity know which frame its from 
		virtual void submitRenderCommandBuffer(ICommandBuffer& commandBuffer) = 0;

		virtual void endFrame() = 0;

	};











}