#pragma once
#include <vector>

namespace TheEngine::Graphics
{
	class IGPURenderSystem
	{
	public:
		virtual ~IGPURenderSystem() = default;

		virtual void initialize() = 0;



		//need to rethink whether to pass reference
		// i am thinking of std::move
		virtual void SubmitRenderComponents(const std::vector<RenderComponent*>& components) = 0;
		virtual void SubmitUIRenderComponents(const std::vector<UIRenderComponent*>& uiComponents) = 0;

		virtual void setCamera(/*camera*/) = 0;

		virtual void RenderFrame() = 0;

	};


} // namespace TheEngine::Graphics
