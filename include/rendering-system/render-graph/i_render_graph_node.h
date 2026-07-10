#pragma once 


namespace TheEngine::RenderingSystem
{
	class  RenderGraphBuilder;

	class IRenderGraphNode
	{


	private:

	public:

		virtual ~IRenderGraphNode() = default;

		virtual void setUp(RenderPassSetupContext& ctx, RenderGraphBuilder& builder) = 0;
		virtual void execute(RenderPassExecuteContext& ctx) = 0;

		virtual void onResize(const WindowExtent& extent, RenderGraphBuilder& builder) = 0;

		virtual std::string getName() const = 0;

	
	};








}